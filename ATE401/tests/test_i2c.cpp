#include "pch.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include "ate401_uart.h"
#include "ate401_i2c.h"

//---------------------------------- ACK 0x00 ----------------------------------//
TEST(ATE401, ACK) {
  std::vector<uint8_t> arr = { '#', '@', '!', 0, static_cast<uint8_t>(ATE401Cmd::ACK) };

  state_i2c.version = 1;
  state_i2c.rxd = 1;
  state_i2c.output = 1;
  state_i2c.rellay = 0;
  state_i2c.buzzer = 1;
  state_i2c.led_red= 1;
  state_i2c.led_green = 0;
  state_i2c.led_blue = 1;
  state_i2c.voltage_TP3 = 3.2;
  state_i2c.voltage_TP13 = 4.8;
  state_i2c.voltage_TP18 = 12;

  const uint8_t* stateBytes = reinterpret_cast<const uint8_t*>(&state);
  arr.insert(arr.end(), stateBytes, stateBytes + sizeof(state));

  arr.insert(arr.begin() + MAGIC.size(), arr.size() - 1); // -1 crc
  arr.push_back(calculateCRC8(arr.data(), arr.size()));

  auto* result = getPackStart(arr.data(), arr.size(), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- TXD 0x01 ----------------------------------//
TEST(ATE401, TXD) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(ATE401Cmd::TXD), 'B', 0};

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}


//---------------------------------- RTE 0x02 ----------------------------------//
// ON
TEST(ATE401, RTE_ON) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(ATE401Cmd::RTE), 1, 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

// OFF
TEST(ATE401, RTE_OFF) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(ATE401Cmd::RTE), 0, 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- DC 0x03 ----------------------------------//
// ON
TEST(ATE401, DC_ON) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(ATE401Cmd::DC), 1, 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

// OFF
TEST(ATE401, DC_OFF) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(ATE401Cmd::DC), 0, 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- TMP 0x03 ----------------------------------//
// ON
TEST(ATE401, TMP_ON) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(ATE401Cmd::TMP), 1, 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  uint8_t* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}
TEST(ATE401, TMP_OFF) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(ATE401Cmd::TMP), 0, 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- BUTTON 0x05 ----------------------------------//
// ON
TEST(ATE401, BUTTON_ON) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(ATE401Cmd::BUTTON), 1, 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

// OFF
TEST(ATE401, BUTTON_OFF) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(ATE401Cmd::BUTTON), 0, 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}
