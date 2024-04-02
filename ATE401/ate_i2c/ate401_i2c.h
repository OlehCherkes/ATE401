#pragma once
#include <cstdint>

extern struct ATE401_i2c state_i2c;

// comands
enum class EEPROM_READ_ADDR : uint16_t
{
  VERSION_L = 0x0000,
  VERSION_H = 0x0001,
  RXD = 0x0002,
  OUTPUT = 0x0003,
  RELAY = 0x0004,
  BUZZER = 0x0005,
  LED_RED_L = 0x0006,
  LED_RED_H = 0x0007,
  LED_GREEN_H = 0x0008,
  LED_GREEN_L = 0x0009,
  LED_BLUE_L = 0x000A,
  LED_BLUE_H = 0x000B,
  VOLTAGE_TP3_L = 0x000C,
  VOLTAGE_TP3_H = 0x000D,
  VOLTAGE_TP13_L = 0x000E,
  VOLTAGE_TP13_H = 0x000F,
  VOLTAGE_TP18_L = 0x0010,
  VOLTAGE_TP18_H = 0x0011,
};

enum class EEPROM_WRITE_ADDR : uint16_t
{
  TXD = 0x0100,
  RTE = 0x0101,
  DC = 0x0102,
  TMP = 0x0103,
  BUTTON = 0x0104,
  CMD_STROB = 0x0105,
};

struct ATE401_i2c {
  uint16_t version;
  uint8_t rxd;
  uint8_t output;
  uint8_t rellay;
  uint8_t buzzer;
  uint8_t led_red;
  uint8_t led_green;
  uint8_t led_blue;
  uint16_t  voltage_TP3;
  uint16_t  voltage_TP13;
  uint16_t  voltage_TP18;
};

bool writeToEEPROM(uint16_t addr, uint8_t val);
uint8_t readFromEEPROM(uint16_t addr);