#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>

// comands
#define ECHO            0x00
#define ACK             0x01
#define TEST_MODE       0x02
#define LED_RED         0x09
#define LED_GREEN       0x0A
#define BUZZER          0x0B
#define SET_TIME        0x0C
#define POWER           0x0F

// states                       
#define STATE_OFF       0x00
#define STATE_ON        0x01
#define STATE_PWM       0x02
#define STATE_BLINK     0x03

extern const std::string MAGIC;
extern const uint8_t crc8Table[256];

uint8_t calculateCRC8(const std::vector<uint8_t>& data);
bool checkCRC8(const std::vector<uint8_t>& data, uint8_t expectedCRC);
std::vector<uint8_t> packed(std::initializer_list<uint8_t> args);
std::vector<uint8_t> unpacked(std::vector<uint8_t>& data);
bool checkCRC8Pack(std::vector<uint8_t>& data);