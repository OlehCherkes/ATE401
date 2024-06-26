#include <iostream>
#include <cstdint>
#include <vector>
#include <cstdarg>
#include <iterator>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include "ate401_uart.h"

/*
=====================================================
Packets struct :
  [MAGIC:3]   | magic bytes '#@!'
  [LENGTH:1]  | packet length with LENGTH & with CRC8
  [PAYLOAD:N] | data N bytes
  [CRC8:1]    | CRC8 from & LENGTH to CRC8
=====================================================
*/

const std::string MAGIC = "#@!";

Mode mode{};
ATE401_uart state;
Esp2MspBlink interval;

// This is the CRC8 polynomial x^8 + x^2 + x^1 + 1 (0x07)
const uint8_t crc8Table[256] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3,
};

uint8_t calculateCRC8(const void* d, size_t len) {
  uint8_t crc = 0;

  auto data = static_cast<const uint8_t*>(d);

  for (int i = 0; i < len; ++i) {
    crc = crc8Table[crc ^ data[i]];
  }

  return crc;
}

bool checkCRC8(const std::vector<uint8_t>& data, uint8_t expectedCRC) {
  uint8_t crc = 0;

  for (const auto& i : data) {
    crc = crc8Table[crc ^ i];
  }

  return crc == expectedCRC;
}

bool checkCRC8Pack(std::vector<uint8_t>& data)
{
  std::vector<uint8_t> checkSrc(data.begin(), data.end() - 1);  // -1 byte size crc
  uint8_t crcResult = calculateCRC8(checkSrc.data(), checkSrc.size());

  if (data.back() != crcResult)
  {
    std::cout << "ERROR, checksum did not match" << std::endl;
    return 0;
  }

  return 1;
}

std::vector<uint8_t> packed(std::initializer_list<uint8_t> args)
{
  std::vector<uint8_t> data;
  data.reserve(MAGIC.size() + args.size() + 2); // memory reservation, +2 bytes lenght and crc 

  std::copy(std::begin(MAGIC), std::end(MAGIC), std::back_inserter(data));

  // insert lenght
  data.push_back(args.size() + 2);  // +2 bytes lenght and crc

  std::copy(std::begin(args), std::end(args), std::back_inserter(data));

  uint8_t crcResult = calculateCRC8(data.data(), data.size());
  data.push_back(crcResult);

  return data;
}

std::vector<uint8_t> packed(const void* args, size_t size) {
  std::vector<uint8_t> data;
  data.reserve(MAGIC.size() + size + 2); // memory reservation, +2 bytes length and crc 

  std::copy(std::begin(MAGIC), std::end(MAGIC), std::back_inserter(data));

  // insert length
  data.push_back(size + 2);  // +2 bytes length and crc

  std::copy(reinterpret_cast<const uint8_t*>(args), reinterpret_cast< const uint8_t*>(args) + size, std::back_inserter(data));

  uint8_t crcResult = calculateCRC8(data.data(), data.size());
  data.push_back(crcResult);

  return data;
}

std::vector<uint8_t> unpacked(std::vector<uint8_t>& data)
{
  std::vector<uint8_t> res(data.begin() + MAGIC.size() + 1, data.end() - 1); // get data without magic number, len and crc

  return res;
}

std::vector<uint8_t> ack(uint8_t cmd, ATE401_uart& state)
{
  std::vector<uint8_t> data;
  data.reserve(MAGIC.size() + 1 + sizeof(state) +  2); // memory reservation

  std::copy(std::begin(MAGIC), std::end(MAGIC), std::back_inserter(data));
  data.push_back(cmd);

  const uint8_t* statePtr = reinterpret_cast<const uint8_t*>(&state);
  std::copy(statePtr, statePtr + sizeof(state), std::back_inserter(data));

  data.insert(data.begin() + MAGIC.size(), data.size());

  uint8_t crcResult = calculateCRC8(data.data(), data.size());
  data.push_back(crcResult);

  return data;
}

uint8_t* getPackStart(const void* data, size_t size, const std::string& substr) {
  const uint8_t* bytes = static_cast<const uint8_t*>(data);
  const uint8_t* subBytes = reinterpret_cast<const uint8_t*>(substr.data());
  size_t subSize = substr.size();

  size_t i = 0;
  while (i < size) {
    if (bytes[i] == subBytes[0] && i + subSize < size) {
      if (std::memcmp(&bytes[i], subBytes, subSize) == 0) {
        uint8_t len = bytes[i + subSize] - 1; // -1 without crc
        uint8_t crc = calculateCRC8(&bytes[i], len + subSize);

        if (crc == bytes[i + subSize + len]) {
          std::cout << "CRC OK = 0x" << std::hex << static_cast<int>(crc) << std::endl;
          return const_cast<uint8_t*>(&bytes[i]);
        }
        else {
          i = i + subSize - 1; // -1 index start from 0
        }
      }
    }
    ++i;
  }

  return nullptr;
}

#if 0
Mode ate401_parser(std::vector<uint8_t>& data)
{
  Pi401Cmd cmd = static_cast<Pi401Cmd>(data.at(0));

  switch (cmd) {
    case Pi401Cmd::ECHO:
      mode.echo = true;
      break;

    case Pi401Cmd::ACK:
   
      break;

    case Pi401Cmd::TEST_MODE:
      mode.ate = data.at(1);
      break;

    case Pi401Cmd::LED_RED:
      mode.led.red = data.at(1);
      break;

    case Pi401Cmd::LED_GREEN:
      mode.led.green = data.at(1);
      break;

    case Pi401Cmd::BUZZER:
      mode.buzzer.state = data.at(1);

      if (mode.buzzer.state == static_cast<uint8_t>(ATE401Indicate::PWM))
      {
        mode.buzzer.properties.count = (static_cast<uint16_t>(data.at(2)) << 8) | static_cast<uint16_t>(data.at(3));
        mode.buzzer.properties.interval_ms = (static_cast<uint16_t>(data.at(4)) << 8) | static_cast<uint16_t>(data.at(5));
        mode.buzzer.properties.duration_ms = (static_cast<uint16_t>(data.at(6)) << 8) | static_cast<uint16_t>(data.at(7));
      }
      break;

    case Pi401Cmd::SET_TIME:
      mode.time = (static_cast<uint32_t>(data.at(2)) << 0) |
                  (static_cast<uint32_t>(data.at(3)) << 8) |
                  (static_cast<uint32_t>(data.at(4)) << 16) |
                  (static_cast<uint32_t>(data.at(5)) << 24);
      break;
  }

  return mode;
}
#endif
