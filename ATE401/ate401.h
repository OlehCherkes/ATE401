#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>

// comands
constexpr uint8_t ECHO           = 0x00;
constexpr uint8_t ACK            = 0x01;
constexpr uint8_t TEST_MODE      = 0x02;
constexpr uint8_t LED_RED        = 0x09;
constexpr uint8_t LED_GREEN      = 0x0A;
constexpr uint8_t BUZZER         = 0x0B;
constexpr uint8_t SET_TIME       = 0x0C;
constexpr uint8_t POWER          = 0x0F;

extern const std::string MAGIC;
extern const uint8_t crc8Table[256];
extern struct Mode mode;
extern struct ATE401State state;

enum class ATE401Indicate : uint8_t
{
  OFF = 0,
  ON = 1,
  PWM = 2,
  BLINK = 3,
};

struct ATE401State {
  uint8_t mvbus : 1;        // power HUB    : normal
  uint8_t mvbat : 1;        // bat   HUB    : normal
  uint8_t wifiConnect : 1;  // eth link     : normal
  uint8_t wifiRssi;         // bat voltage in mV
  uint8_t gsmRssi;          // bat voltage in mV

  uint8_t batMv[2];         // bat voltage in mV
  uint8_t wifiIp[4];        // ip over wifi
};

struct Mode {
  bool echo;
  uint8_t ate;

  struct LED
  {
    uint8_t red;
    uint8_t green;
  } led;


  struct Buzzer
  {
    uint8_t state;

    struct Properties {
      uint16_t count;
      uint16_t interval_ms;
      uint16_t duration_ms;
    } properties;
  } buzzer;

  uint32_t time;
  uint16_t power_delay_ms;
};

uint8_t calculateCRC8(uint8_t* data, size_t len);
bool checkCRC8(const std::vector<uint8_t>& data, uint8_t expectedCRC);
bool checkCRC8Pack(std::vector<uint8_t>& data);
std::vector<uint8_t> packed(std::initializer_list<uint8_t> args);
std::vector<uint8_t> unpacked(std::vector<uint8_t>& data);
Mode ate401_parser(std::vector<uint8_t>& data);
std::vector<uint8_t> ack(uint8_t cmd, ATE401State& state);