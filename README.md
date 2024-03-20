# Protocol ATE401 'Raspberry Pi' <-> 'ATE401' | 19.03.2024

IPL License

Copyright 2024 U-Prox by Oleh Cherkes

Permission is hereby granted, to the employees of U-Prox company.



* [Overview](#chapter-0)
* [Transport part](#chapter-1)
* [Command part](#chapter-2)
* [Examples of using commands](#chapter-3)
  * [GET_STATE](#chapter-4)
  * [RESPONSE_STATE](#chapter-5)
  * [LED_RED](#chapter-6)
  * [LED_GREEN](#chapter-7)
  * [BUZZER](#chapter-8)
  * [SET_TIME](#chapter-9)
  * [ACK](#chapter-10)
  * [MODEM_DTR](#chapter-11)
  * [POWER](#chapter-12)
  * [WATCHDOG](#chapter-13)
  * [ACK_FW](#chapter-14)
  * [MODEM_SIM](#chapter-15)
* [Reply](#chapter-16)
* [Optional structures](#chapter-17)

<a id="chapter-0"></a>
Overview
===============================================================================================================================
The data exchange protocol between Raspberry Pi and test equipment is IP401. This protocol facilitates seamless communication and data transfer between the Raspberry Pi and the testing equipment. It specifies the format, structure, and rules for transmitting and receiving data, allowing the Raspberry Pi and the test equipment to understand and interpret the information exchanged between them accurately.

<a id="chapter-1"></a>
Transport part
===============================================================================================================================
```markdown
Packets struct:
  [MAGIC:3]               | magic bytes '#@!'
  [LENGTH:1]              | packet length with LENGTH & with CRC8
  [PAYLOAD:N]             | data N bytes
  [CRC8:1]                | CRC8 from &LENGTH to CRC8
```

<a id="chapter-2"></a>
Commands part
===============================================================================================================================

```markdown
  GET_STATE         = 0x07
  RESPONSE_STATE    = 0x08
  LED_RED           = 0x09
  LED_GREEN         = 0x0A
  BUZZER            = 0x0B
  SET_TIME          = 0x0C
  ACK               = 0x0D
  MODEM_DTR         = 0x0E
  POWER             = 0x0F
  WATCHDOG          = 0x10
  ACK_FW            = 0x11
  MODEM_SIM         = 0x12
  ```

<a id="chapter-2"></a>
Examples of using commands
=============================================================================================================================== 
<a id="chapter-4"></a>
**GET_STATE**       0x07
```markdown
  [MAGIC:3][LENGTH:1][GET_STATE:1][CRC8:1]
```
  * After sending this command, the device under test sends a response with a description of the device state, various parameters and flags.
    - version
    - time
    - bat
    - modem DCD
    - modem status
    - tamper
    - power
    - power key
    - pwm Red Led
    - pwm Green Led
    - buzzer Red Led

---------------------------------

<a id="chapter-6"></a>
???
**LED_RED**           0x09
```markdown
  [MAGIC:3][LENGTH:1][LED_RED:1][STATE:1][CRC8:1]
```
* STATE
    * OFF = 0
    * ON = 1
---------------------------------

<a id="chapter-7"></a>
**LED_GREEN**         0x0A
```markdown
  [MAGIC:3][LENGTH:1][LED_GREEN:1][STATE:1][CRC8:1]
```
* STATE
    * OFF = 0
    * ON = 1
---------------------------------

<a id="chapter-8"></a>
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

<a id="chapter-9"></a>
**SET_TIME**         0x0С

```markdown
  [MAGIC:3][LENGTH:1][SET_TIME:1][TIME:4|Little-Endian][CRC8:1]
```

Used [UnixTime](https://en.wikipedia.org/wiki/Unix_time)

```markdown
uint32_t time_le = 1616183220; // time Little-Endian format
uint8_t time_bytes[4];

time_bytes[0] = (time_le >> 0) & 0xFF;
time_bytes[1] = (time_le >> 8) & 0xFF;
time_bytes[2] = (time_le >> 16) & 0xFF;
time_bytes[3] = (time_le >> 24) & 0xFF;
```
---------------------------------

<a id="chapter-10"></a>
???
**ACK**               0x0D

ACK response ESP32 to any data from Raspberry Pi
```markdown
  [MAGIC:3][LENGTH:1][ACK:1][STATUS ???][CRC8:1]
```
---------------------------------

<a id="chapter-11"></a>
**MODEM_DTR**          0x0E

```markdown
  [MAGIC:3][LENGTH:1][MODEM_DTR:1][STATE:1][CRC8:1]
```

  * STATE
    * OFF = HIGH
    * ON = LOW
---------------------------------

<a id="chapter-12"></a>
**POWER**             0x0F

POWER **OFF** after delay in ms
```markdown
  [MAGIC:3][LENGTH:1][POWER:1][DELAY_MS:2||Little-Endian][CRC8:1]
```
```markdown
// delay ms Little-Endian format

uint16_t value = 1000;
uint8_t byte1 = value & 0xFF;
uint8_t byte2 = (value >> 8) & 0xFF;
```
---------------------------------

<a id="chapter-13"></a>
**WATCHDOG**          0x10

WATCHDOG **OFF** after delay in sec

```markdown
  [MAGIC:3][LENGTH:1][WATCHDOG:1][TIME_SEC:2][CRC8:1]
```

  * TIME_SEC
    * OFF = 0
    * Time keepalive in sec = !0 
---------------------------------

<a id="chapter-14"></a>
**ACK_FW**             0x11

ACK_FW firmware update result response ESP32 to any data from Raspberry Pi

```markdown
  [MAGIC:3][LENGTH:1][ACK_FW:1][STATE:1][CRC8:1]
```

  * STATE
    * SUCCES = 0
    * ERROR = !0 
---------------------------------

<a id="chapter-15"></a>
???
**MODEM_SIM**           0x12

Modem **power ON**

```markdown
  [MAGIC:3][LENGTH:1][MODEM_SIM:1][SIM_CARD:1][CRC8:1]
```

  * STATE
    * SimCard = 0x00
---------------------------------

<a id="chapter-16"></a>
Reply
===============================================================================================================================
Upon successful delivery and reading of the command, the testing equipment provides a response indicating either success or failure.

Response to the command **GET_STATE 0x07**, you will receive a data packet of this format.

<a id="chapter-5"></a>
???
**RESPONSE_STATE**    0x08
```markdown
  [MAGIC:3][LENGTH:1][RESPONSE_STATE:1][CRC8:1]
```
The data will come in the form of a structure:
```c++
struct ATE401State {
    uint8_t version[2];       // version
    uint8_t time[4];          // time, uint32_t   : le, sec from last start
    uint8_t mvbat[2];         // bat, uint16_t    : le, in mV, 0 == battary absent
    
    uint8_t modemDCD : 1;     // modem DCD        : 1 == HIGH, 0 == LOW
    uint8_t resedved_1 : 1;   // reserved         : 1 == HIGH, 0 == LOW
    uint8_t modemStatus : 1;  // modem status     : 1 == HIGH, 0 == LOW
    uint8_t tamper : 1;       // tamper           : 1 == HIGH, 0 == LOW
    uint8_t mvbus : 1;        // power            : 1 == POWER ON, 0 == POWER OFF
    uint8_t powerKey : 1;     // power key        : 1 == HIGH, 0 == LOW
    uint8_t resedved_6_7 : 2; // reserved         : 1 == HIGH, 0 == LOW
    
    uint8_t pwmRed : 1;       // pwm Red Led      : 1 == Progress, 0 == LOW
    uint8_t pwmGreen : 1;     // pwm Green Led    : 1 == Progress, 0 == LOW
    uint8_t buzzer : 1;       // buzzer Red Led   : 1 == Progress, 0 == LOW
  };
```
<a id="chapter-7"></a>
Optional structures
===============================================================================================================================

```c++
  enum class PiATE401Cmd : uint8_t
  {
    MODEM_POWER = 0x01,
    MODEM_POWERKEY = 0x02,
    SRD = 0x03,
    ETH = 0x04,
    FW = 0x05,
    FW_CRC = 0x06,
    GET_STATE = 0x07,
    RESPONSE_STATE = 0x08,
    LED_RED = 0x09,
    LED_GREEN = 0x0A,
    BUZZER = 0x0B,
    SET_TIME = 0x0C,
    ACK = 0x0D,
    MODEM_DTR = 0x0E,
    POWER = 0x0F,
    WATCHDOG = 0x10,
    ACK_FW = 0x11,
    MODEM_SIM = 0x12,
  };
```
```c++
  enum class PiATE401Param : uint8_t
  {
    OFF = 0,
    ON = 1,
  };
```
```c++
  enum class PiATE401Indicate : uint8_t
  {
    OFF = 0,
    ON = 1,
    PWM = 2,
    BLINK = 3,
  };
```
```c++
  struct PiATE401Pwm
  {
    uint8_t startLevel;
    uint8_t endLevel;
    uint8_t duartionSec;
  };
```
```c++
  struct PiATE401Blink
  {
    // count intervals == 0 (infinity repeate)
    uint16_t count;
    uint16_t intervalMs;
    uint16_t duartionMs;
  };
  ```