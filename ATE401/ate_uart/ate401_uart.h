#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>

extern const std::string MAGIC;
extern const uint8_t crc8Table[256];
extern struct Mode mode;
extern struct ATE401_uart state;
extern struct Esp2MspBlink interval;

// comands
enum class Pi401Cmd : uint8_t
{
  ECHO       = 0x00,
  ACK        = 0x01,
  TEST_MODE  = 0x02,
  SET_TIME   = 0x03,
  RXD        = 0x04,
  OUT        = 0x05,
  REL        = 0x06,
  BUZZER     = 0x07,
  LED_RED    = 0x08,
  LED_GREEN  = 0x09,
  LED_BLUE   = 0x0A,
  WIFI_CRED  = 0x0B,
};

enum class ATE401Indicate : uint8_t
{
  OFF = 0,
  ON = 1,
  PWM = 2,
  BLINK = 3,
};

struct Esp2MspBlink
{
  // count intervals, count == 0 infinity repeate
  uint16_t count;
  uint16_t intervalMs;
  uint16_t duartionMs;
};

struct ATE401_uart {
  uint16_t version;
  uint32_t time;
  uint8_t txd;
  uint8_t request_to_exit;
  uint8_t door_control;
  uint8_t tamper;
  uint8_t button;
  uint32_t ip_address;
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

uint8_t calculateCRC8(const void* d, size_t len);
bool checkCRC8(const std::vector<uint8_t>& data, uint8_t expectedCRC);
bool checkCRC8Pack(std::vector<uint8_t>& data);
std::vector<uint8_t> packed(std::initializer_list<uint8_t> args);
std::vector<uint8_t> packed(const void* args, size_t size);
std::vector<uint8_t> unpacked(std::vector<uint8_t>& data);
Mode ate401_parser(std::vector<uint8_t>& data);
std::vector<uint8_t> ack(uint8_t cmd, ATE401_uart& state);
uint8_t* getPackStart(const void* data, size_t size, const std::string& substr);