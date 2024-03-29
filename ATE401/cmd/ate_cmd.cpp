#include <iostream>
#include <algorithm>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include "ate401.h"

void measurements(Ate401Cmd cmd)
{
  // send cmd to RS-485
}


//-------------------------- Time --------------------------//
std::vector<uint8_t> time_le(const uint32_t tm) {
  std::vector<uint8_t> time_bytes(4);

  time_bytes[0] = (tm >> 0) & 0xFF;
  time_bytes[1] = (tm >> 8) & 0xFF;
  time_bytes[2] = (tm >> 16) & 0xFF;
  time_bytes[3] = (tm >> 24) & 0xFF;

  return time_bytes;
}

void getTime(const void* data) {
  const uint8_t* t = reinterpret_cast<const uint8_t*>(data);

  uint32_t time_le =
    (static_cast<uint32_t>(t[0]) << 0) |
    (static_cast<uint32_t>(t[1]) << 8) |
    (static_cast<uint32_t>(t[2]) << 16) |
    (static_cast<uint32_t>(t[3]) << 24);

  time_t rawTime = static_cast<time_t>(time_le);

  struct tm timeinfo;
  if (gmtime_s(&timeinfo, &rawTime) == 0) {
    std::cout << "Time: ";
    std::cout << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S UTC") << std::endl;
  }
  else {
    std::cerr << "Time conversion error" << std::endl;
  }
}

//-------------------------- TXD --------------------------//
void setTXD()
{
  // send comandvia i2c
}

void getTXD()
{
  packed({ static_cast<uint8_t>(Ate401Cmd::TXD) });
}

//-------------------------- RXD --------------------------//
void setRXD(const uint8_t data)
{
  packed({ static_cast<uint8_t>(Ate401Cmd::RXD), data });

}

void getRXD()
{
  // read via i2c
}

//-------------------------- OUT --------------------------//
// PI -> ip401
void setOUT(const uint8_t state)
{
  packed({ static_cast<uint8_t>(Ate401Cmd::OUT), state });
}

// pi401 -> ATE
void getOUT(uint8_t state)
{
  if (state) {
    // ON pin
  }
  else {
    // OFF pin
  }


  measurements(Ate401Cmd::OUT);
}

//-------------------------- RTE --------------------------//
void setRTE()
{
  packed({ static_cast<uint8_t>(Ate401Cmd::RXD), 'A' });

}

void getRTE()
{

}

//-------------------------- DC --------------------------//
void setDC()
{
  packed({ static_cast<uint8_t>(Ate401Cmd::RXD), 'A' });

}

void getDC()
{

}

//-------------------------- REL --------------------------//
void setREL()
{
  packed({ static_cast<uint8_t>(Ate401Cmd::RXD), 'A' });

}

void getREL()
{

}

//-------------------------- TMP --------------------------//
void setTMP()
{
  packed({ static_cast<uint8_t>(Ate401Cmd::RXD), 'A' });

}

void getTMP()
{

}

//-------------------------- BUTTON --------------------------//
void setBUTTON()
{
  packed({ static_cast<uint8_t>(Ate401Cmd::RXD), 'A' });

}

void getBUTTON()
{

}

//-------------------------- BUZZER --------------------------//
void setBUZZER()
{
  packed({ static_cast<uint8_t>(Ate401Cmd::RXD), 'A' });

}

void getBUZZER()
{

}