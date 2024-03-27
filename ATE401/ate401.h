#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>

extern const std::string MAGIC;
extern const uint8_t crc8Table[256];
extern struct Mode mode;
extern struct ATE401State state;
extern struct Esp2MspBlink interval;

// comands
enum class Ate401Cmd: uint8_t
{
  ECHO           = 0x00,
  ACK            = 0x01,
  TEST_MODE      = 0x02,
  SET_TIME       = 0x03,
  TXD            = 0x04,
  RXD            = 0x05,
  OUT            = 0x06,
  RTE            = 0x07,
  DC             = 0x08,
  REL            = 0x09,
  TMP            = 0x0A,
  BUTTON         = 0x0B,
  BUZZER         = 0x0C,
  LED_RED        = 0x0D,
  LED_GREEN      = 0x0E,
  LED_BLUE       = 0x0F,
  WIFI_CRED      = 0x10,

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

struct ATE401State {
  uint16_t version;
  uint32_t time;
  uint8_t txd;
  uint8_t rxd;
  uint8_t output;
  uint8_t request_to_exit;
  uint8_t door_control;
  uint8_t rellay;
  uint8_t tamper;
  uint8_t button;
  uint8_t buzzer;
  uint8_t led_red;
  uint8_t led_green;
  uint8_t led_blue;
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
std::vector<uint8_t> ack(uint8_t cmd, ATE401State& state);