#include <iostream>
#include <string>
#include <cstdint>
#include "ATE401/ate401.h"

int main(void)
{
  // test
  std::vector<uint8_t> data = { 0x23, 0x40, 0x21, 0x07, 0x09, 0x01 };

  uint8_t crcResult = calculateCRC8(data);

  std::cout << "CRC8: 0x" << std::hex << static_cast<int>(crcResult) << std::endl;

  checkCRC8(data, crcResult) ? std::cout << "OK" << std::endl : std::cout << "ERROR" << std::endl;


 //----------------------- PACK -------------------------------
  std::cout << std::endl;

  std::vector <uint8_t> res = packed({ LED_RED, STATE_ON });

  std::cout << "Packed elements:" << std::endl;
  for (const auto& element : res) {
    std::cout << "0x" << std::hex << static_cast<int>(element) << " ";
  }
  std::cout << std::endl;
  

  //---------------------- UNPACK ------------------------------ 
  std::cout << std::endl;

  if (checkCRC8Pack(res))
  {
    res = unpacked(res);

    std::cout << "Unpacked elements:" << std::endl;
    for (const auto& element : res) {
      std::cout << "0x" << std::hex << static_cast<int>(element) << " ";
    }
    std::cout << std::endl;
  }

	return 0;
}