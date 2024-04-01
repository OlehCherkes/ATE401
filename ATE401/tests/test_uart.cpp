#include "pch.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include "ate401_uart.h"

//---------------------------------- ECHO 0x00 ----------------------------------//
TEST(ATE401, ECHO) {
  uint8_t arr[] = { '#', '@', '!', 3, static_cast<uint8_t>(Pi401Cmd::ECHO), 0 };

  const auto s = std::size(arr) - 1; // -1 crc
  arr[s] = calculateCRC8(&arr[0], s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- ACK 0x01 ----------------------------------//
TEST(ATE401, ACK) {
  std::vector<uint8_t> arr = { '#', '@', '!', 0, static_cast<uint8_t>(Pi401Cmd::ACK)};

  state.version = 1;
  state.time = 1616183220;
  state.txd = 1;
  state.request_to_exit = 4;
  state.door_control = 5;
  state.tamper = 7;
  state.button = 8;
  state.ip_address = 0x0A0B0C0D;

  const uint8_t* stateBytes = reinterpret_cast<const uint8_t*>(&state);
  arr.insert(arr.end(), stateBytes, stateBytes + sizeof(state));

  arr.insert(arr.begin() + MAGIC.size(), arr.size() - 1); // -1 crc
  arr.push_back(calculateCRC8(arr.data(), arr.size()));

  auto* result = getPackStart(arr.data(), arr.size(), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- TEST_MODE 0x02 ----------------------------------//
// ON
TEST(ATE401, TEST_MODE_ON) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::TEST_MODE), static_cast<uint8_t>(ATE401Indicate::ON), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

// OFF
TEST(ATE401, TEST_MODE_OFF) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::TEST_MODE), static_cast<uint8_t>(ATE401Indicate::OFF), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- SET_TIME 0x03 ----------------------------------//
TEST(ATE401, SET_TIME) {
  uint8_t arr[] = {'#', '@', '!', 7, static_cast<uint8_t>(Pi401Cmd::SET_TIME), 0, 0, 0, 0, 0 };

  // Packing time to Little-Endian format
  uint32_t time_le = 1616183220;
  uint8_t time_bytes[4];

  time_bytes[0] = (time_le >> 0) & 0xFF;
  time_bytes[1] = (time_le >> 8) & 0xFF;
  time_bytes[2] = (time_le >> 16) & 0xFF;
  time_bytes[3] = (time_le >> 24) & 0xFF;

  arr[5] = time_bytes[0];
  arr[6] = time_bytes[1];
  arr[7] = time_bytes[2];
  arr[8] = time_bytes[3];

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- RXD 0x04 ----------------------------------//
TEST(ATE401, RXD) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::RXD), 'A', 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- OUT 0x05 ----------------------------------//
// ON
TEST(ATE401, OUT_ON) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::OUT), static_cast<uint8_t>(ATE401Indicate::ON), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

// OFF
TEST(ATE401, OUT_OFF) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::OUT), static_cast<uint8_t>(ATE401Indicate::OFF), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- REL 0x06 ----------------------------------//
// ON
TEST(ATE401, REL_ON) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::REL), static_cast<uint8_t>(ATE401Indicate::ON), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

// OFF
TEST(ATE401, REL_OFF) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::REL), static_cast<uint8_t>(ATE401Indicate::OFF), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- BUZZER 0x07 ----------------------------------//
// ON
TEST(ATE401, BUZZER_ON) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::BUZZER), static_cast<uint8_t>(ATE401Indicate::ON), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

// OFF
TEST(ATE401, BUZZER_OFF) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::BUZZER), static_cast<uint8_t>(ATE401Indicate::OFF), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

// PWM
TEST(ATE401, BUZZER_PWM) {
  uint8_t arr[] = { '#', '@', '!', 10, static_cast<uint8_t>(Pi401Cmd::BUZZER), static_cast<uint8_t>(ATE401Indicate::PWM),
                    interval.count & 0xFF, (interval.count >> 8) & 0xFF,
                    interval.intervalMs & 0xFF, (interval.intervalMs >> 8) & 0xFF,
                    interval.duartionMs & 0xFF, (interval.duartionMs >> 8) & 0xFF, 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

// BLINK 
TEST(ATE401, BUZZER_BLINK) {
  uint8_t arr[] = { '#', '@', '!', 10, static_cast<uint8_t>(Pi401Cmd::BUZZER), static_cast<uint8_t>(ATE401Indicate::BLINK),
                    interval.count & 0xFF, (interval.count >> 8) & 0xFF,
                    interval.intervalMs & 0xFF, (interval.intervalMs >> 8) & 0xFF,
                    interval.duartionMs & 0xFF, (interval.duartionMs >> 8) & 0xFF, 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- LED_RED 0x08 ----------------------------------//
// ON
TEST(ATE401, LED_RED_ON) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::LED_RED), static_cast<uint8_t>(ATE401Indicate::ON), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  void* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

// OFF
TEST(ATE401, LED_RED_OFF) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::LED_RED), static_cast<uint8_t>(ATE401Indicate::OFF), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- LED_GREEN 0x09 ----------------------------------//
// ON
TEST(ATE401, LED_GREEN_ON) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::LED_GREEN), static_cast<uint8_t>(ATE401Indicate::ON), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  void* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

// OFF
TEST(ATE401, LED_GREEN_OFF) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::LED_RED), static_cast<uint8_t>(ATE401Indicate::OFF), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- LED_BLUE 0x0A ----------------------------------//
// ON
TEST(ATE401, LED_BLUE_ON) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::LED_BLUE), static_cast<uint8_t>(ATE401Indicate::ON), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

// OFF
TEST(ATE401, LED_BLUE_OFF) {
  uint8_t arr[] = { '#', '@', '!', 4, static_cast<uint8_t>(Pi401Cmd::LED_BLUE), static_cast<uint8_t>(ATE401Indicate::OFF), 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

//---------------------------------- WIFI_CRED 0x0B ----------------------------------//
TEST(ATE401, WIFI_CRED) {
  constexpr uint8_t NTS = 0x00;

  uint8_t arr[] = { '#', '@', '!', 12, static_cast<uint8_t>(Pi401Cmd::WIFI_CRED), 'I', 'T', 'V', NTS, 'P', 'S', 'W', 'D', NTS, 0 };

  const auto s = std::size(arr) - 1;
  arr[s] = calculateCRC8(arr, s);

  auto* result = getPackStart(&arr[0], std::size(arr), MAGIC);
  ASSERT_NE(result, nullptr);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}