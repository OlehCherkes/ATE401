#include <iostream>
#include <cstdint>
#include "ate401_i2c.h"

/*
=====================================================
Packets struct :
  [MAGIC:3]   | magic bytes '#@!'
  [LENGTH:1]  | packet length with LENGTH & with CRC8
  [PAYLOAD:N] | data N bytes
  [CRC8:1]    | CRC8 from & LENGTH to CRC8
=====================================================
*/

ATE401_i2c state_i2c;