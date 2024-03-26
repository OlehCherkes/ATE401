# Protocol ATE401 'Raspberry Pi' <-> 'ATE401' | 19.03.2024

IPL License

Copyright 2024 U-Prox

Permission is hereby granted, to the employees of U-Prox company.

* [Overview](#chapter-0)
* [Transport part](#chapter-1)
* [Command part](#chapter-2)
* [Examples of using commands](#chapter-3)
  * [ECHO](#chapter-4)
  * [ACK](#chapter-5)
  * [TEST_MODE](#chapter-6)
  * [LED_RED](#chapter-7)
  * [LED_GREEN](#chapter-8)
  * [BUZZER](#chapter-9)
  * [SET_TIME](#chapter-10)
  * [POWER](#chapter-11)
  
* [Reply](#chapter-12)
* [Test points](#chapter-14)
* [Optional structures](#chapter-15)

<a id="chapter-0"></a>
Overview
===============================================================================================================================
The data exchange protocol between Raspberry Pi and test equipment is IP401. This protocol facilitates seamless communication and data transfer between the Raspberry Pi and the testing equipment. It specifies the format, structure, and rules for transmitting and receiving data, allowing the Raspberry Pi and the test equipment to understand and interpret the information exchanged between them accurately.

![ate](/doc/ate.png)

<a id="chapter-1"></a>
Transport part
===============================================================================================================================
```markdown
Packets struct:
  [MAGIC:3]               | magic bytes '#@!'
  [LENGTH:1]              | length includes data size, LENGTH and CRC8 (without magic bytes)
  [PAYLOAD:N]             | data N bytes
  [CRC8:1]                | CRC8 from LENGTH to CRC8
```

<a id="chapter-2"></a>
Commands part
===============================================================================================================================

```
  ECHO              = 0x00
  ACK               = 0x01
  TEST_MODE         = 0x02
  TIME              = 0x03
  POWER 3.3         = 0x04
  TXD               = 0x05
  RXD               = 0x06
  OUT               = 0x07
  RTE               = 0x08
  DC                = 0x09
  POWER 4.8         = 0x0A
  REL               = 0x0B
  BUZZER            = 0x0C
  POWER 12          = 0x0D
  TMP               = 0x0E
  CAP_BUTTON        = 0x0F
  LED_RED           = 0x10
  LED_GREEN         = 0x11
  POWER             = 0x12
  ```

<a id="chapter-3"></a>
Examples of using commands
=============================================================================================================================== 

<a id="chapter-4"></a>
**ECHO**              0x00

Start/End TestMode
```markdown
  [MAGIC:3][LENGTH:1][ECHO:1][CRC8:1]
```
---------------------------------

<a id="chapter-5"></a>
**ACK**              0x01

```markdown
  [MAGIC:3][LENGTH:1][ACK:1][ATE401State:sizeoff(ATE401State)][CRC8:1]
```

After sending this command, the device under test sends a response with a description of the device state, various parameters and flags.
    
[ATE401State structure ](#chapter-13)
  - Version
  - Time
  - TXD
  - RXD
  - OUT
  - RTE
  - DC
  - REL
  - Buzzer
  - TMP
  - Capacitive Button
  - Battery
  - Power 3.3
  - Power 4.8
  - Power 12
  - Led Red 
  - Led Green
  - Led Blue  
---------------------------------

<a id="chapter-6"></a>
**TEST_MODE**         0x02

Start/End TestMode
```markdown
  [MAGIC:3][LENGTH:1][TEST_MODE:1][ATE_PARAM:1][CRC8:1]
```
* ATE_PARAM
    * OFF = 0
    * ON = 1
---------------------------------

<a id="chapter-7"></a>
**LED_RED**           0x09
```markdown
  [MAGIC:3][LENGTH:1][LED_RED:1][STATE:1][CRC8:1]
```
* STATE
    * OFF = 0
    * ON = 1
---------------------------------

<a id="chapter-8"></a>
**LED_GREEN**         0x0A
```markdown
  [MAGIC:3][LENGTH:1][LED_GREEN:1][STATE:1][CRC8:1]
```
* STATE
    * OFF = 0
    * ON = 1
---------------------------------

<a id="chapter-9"></a>
**BUZZER**           0x0B
```markdown
  [MAGIC:3][LENGTH:1][BUZZER:1][STATE:1][CRC8:1]
```
* STATE
    * OFF = 0
    * ON = 1
    * PWM = 2
    * BLINK = 3

*if the state is PWM or BLINK, this packet format is used
```markdown
  [MAGIC:3][LENGTH:1][BUZZER:1][STATE:1][PROPERTIES:6][CRC8:1]
```

* PROPERTIES
    * COUNT : 2
    * INTERVAL_MS : 2
    * DURATION_MS : 2

    ** count intervals == 0 (infinity repeate)
---------------------------------

<a id="chapter-10"></a>
**SET_TIME**         0x0С

```markdown
  [MAGIC:3][LENGTH:1][SET_TIME:1][TIME:4|Little-Endian][CRC8:1]
```

Used [UnixTime](https://en.wikipedia.org/wiki/Unix_time)

```markdown
// Packing time to Little-Endian format
uint32_t time_le = 1616183220; 
uint8_t time_bytes[4];

time_bytes[0] = (time_le >> 0) & 0xFF;
time_bytes[1] = (time_le >> 8) & 0xFF;
time_bytes[2] = (time_le >> 16) & 0xFF;
time_bytes[3] = (time_le >> 24) & 0xFF;

// Unpacking from Little-Endian format to uint32_t
uint32_t time_le =
    (static_cast<uint32_t>(time_bytes[0]) << 0) |
    (static_cast<uint32_t>(time_bytes[1]) << 8) |
    (static_cast<uint32_t>(time_bytes[2]) << 16) |
    (static_cast<uint32_t>(time_bytes[3]) << 24);
```
---------------------------------

<a id="chapter-11"></a>
**POWER**             0x0F

POWER **OFF** after delay in ms
```markdown
  [MAGIC:3][LENGTH:1][POWER:1][DELAY_MS:2||Little-Endian][CRC8:1]
```
```markdown
// Packing delay ms to Little-Endian format
uint16_t value = 1000;
uint8_t byte1 = value & 0xFF;
uint8_t byte2 = (value >> 8) & 0xFF;

// Unpacking delay ms from Little-Endian format to uint16_t
uint16_t value = (static_cast<uint32_t>(time_bytes[0]) << 0) | (static_cast<uint32_t>(time_bytes[1]) << 8);
```
---------------------------------

<a id="chapter-12"></a>
Reply
===============================================================================================================================
Upon successful delivery and reading of the command, the testing equipment provides a response indicating either success or failure.

Response to the command **ACK 0x01**, you will receive a data packet of this format.

<a id="chapter-13"></a>
The data will come in the form of a structure:
```c++
struct ATE401State {
    uint8_t mvbus : 1;        // power HUB    : normal
    uint8_t mvbat : 1;        // bat   HUB    : normal
    uint8_t gsm_sim_det : 1;  // modem        : sim card present
    uint8_t gsm_register : 1; // modem        : registered
    uint8_t boff : 1;         // HUB power key: pressed
    uint8_t tamper : 1;       // HUB tamper   : normal
    uint8_t rf0 : 1;          // error
    uint8_t rf1 : 1;          // error
    uint8_t eth : 1;          // error
    uint8_t ethLink : 1;      // eth link     : normal
    uint8_t wifiConnect : 1;  // eth link     : normal
    uint8_t wifiRssi;         // bat voltage in mV
    uint8_t gsmRssi;          // bat voltage in mV

    uint8_t batMv[2];         // bat voltage in mV
    uint8_t ethIp[4];         // ip over ethernet
    uint8_t wifiIp[4];        // ip over wifi
  };
```

<a id="chapter-14"></a>
Test points
===============================================================================================================================
  The value of the ip401 gpio outputs measures the test equipment, after which the data can be read via I2C. The value from the gpio outputs is measured by ip401, it can be read via UART.

```markdown
  TP1 - clock calibration
  TP2 - GND
  TP3 - get the voltage value from the test equipment via the I2C interface (normally 3.3v)
  TP4 - for programming
  TP5 - TX (Raspberry Pi <-> ip401)
  TP6 - RX (Raspberry Pi <-> ip401)
  TP7 - for programming (boot mode)
  TP8 - TXD (write byte via I2C, receiving the same byte via TX)
  TP9 - RXD (write byte via RX, receiving the same byte via I2C)
  TP10 - OUT (send command on/off via UART, read value via I2C)
  TP11 - RTE (send command on/off via I2c, read value via UART)
  TP12 - DC (send command on/off via I2c, read value via UART)
  TP13 - reading voltage data via the I2C interface (normally 4.8v)
  TP14 - REL (send command on/off via UART, read value via I2C)
  TP15 - for programming
  TP16 - GND
  TP17 - BUZZER (send command on/off via UART, read value via I2C)
  TP18 - +12v3 (reading voltage data via the I2C interface)
  TP19 - TMP (send command on/off via I2c, read value via UART)
  TP20 - Capacitive Button (send command on/off via UART, read value via I2C)
  TP21 - JTDO for programming
  TP22 - TDI for JTAG (not used) 

  LED - RED and GREEN (send command on/off via I2c, read value via UART)
```

<a id="chapter-15"></a>
Optional structures
===============================================================================================================================

```c++
  enum class PiATE401Cmd : uint8_t
  {
    ECHO = 0x00,
    ACK = 0x01,
    TEST_MODE = 0x02,
    LED_RED = 0x09,
    LED_GREEN = 0x0A,
    BUZZER = 0x0B,
    SET_TIME = 0x0C,
    POWER = 0x0F,
  };
```
```c++
  enum class ATE401Param : uint8_t
  {
    OFF = 0,
    ON = 1,
  };
```
```c++
  enum class ATE401Indicate : uint8_t
  {
    OFF = 0,
    ON = 1,
    PWM = 2,
    BLINK = 3,
  };
```
```c++
  struct ATE401Pwm
  {
    uint8_t startLevel;
    uint8_t endLevel;
    uint8_t duartionSec;
  };
```
```c++
  struct ATE401Blink
  {
    // count intervals == 0 (infinity repeate)
    uint16_t count;
    uint16_t intervalMs;
    uint16_t duartionMs;
  };
  ```