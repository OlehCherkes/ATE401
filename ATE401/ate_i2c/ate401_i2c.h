#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include "ate401_uart.h"

extern struct ATE401_i2c state_i2c;

// comands
enum class ATE401Cmd : uint8_t
{
  ACK        = 0x00,
  TXD        = 0x01,
  RTE        = 0x02,
  DC         = 0x03,
  TMP        = 0x04,
  BUTTON     = 0x05,
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
  float voltage_TP3;
  float voltage_TP13;
  float voltage_TP18;
};