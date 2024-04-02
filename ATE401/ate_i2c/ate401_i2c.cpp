#include <iostream>
#include <cstdint>
#include "ate401_i2c.h"

/*
=====================================================
Packets struct:
  [MEMORY AREA:1]         | EEPROM memory area
  [STATE:1]               | write data
=====================================================
*/

ATE401_i2c state_i2c;

bool writeToEEPROM(uint16_t addr, uint8_t val) {

  if (reinterpret_cast<uint16_t*>(&addr) == nullptr)
  {
    return false;
  }

  std::cout << "Write value " << std::hex << static_cast<int>(val) << " to EEPROM address " << std::hex << addr << std::endl;

  return true;
}

uint8_t readFromEEPROM(uint16_t addr) {
  uint8_t value = 0xFF;

  std::cout << "Read value " << std::hex << static_cast<int>(value) << " from EEPROM address " << std::hex << addr << std::endl;

  return value;
}