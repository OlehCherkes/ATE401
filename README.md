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
  * [SET_TIME](#chapter-7)
  * [TXD](#chapter-8)
  * [RXD](#chapter-9)
  * [OUT](#chapter-10)
  * [RTE](#chapter-11)
  * [DC](#chapter-12)
  * [REL](#chapter-13)
  * [TMP](#chapter-14)
  * [BUTTON](#chapter-15)
  * [BUZZER](#chapter-16)
  * [LED_RED](#chapter-17)
  * [LED_GREEN](#chapter-18)
  * [LED_BLUE](#chapter-19)
  * [WIFI_CRED](#chapter-20)
  
* [Reply](#chapter-21)
* [Test points](#chapter-22)
* [Optional structures](#chapter-23)

<a id="chapter-0"></a>
Overview
===============================================================================================================================
The data exchange protocol between Raspberry Pi and test equipment is IP401. This protocol facilitates seamless communication and data transfer between the Raspberry Pi and the testing equipment. It specifies the format, structure, and rules for transmitting and receiving data, allowing the Raspberry Pi and the test equipment to understand and interpret the information exchanged between them accurately.

To get started, the device needs to be put into test mode by sending the command **TEST_MODE**

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
  SET_TIME          = 0x03
  TXD               = 0x04
  RXD               = 0x05
  OUT               = 0x06
  RTE               = 0x07
  DC                = 0x08
  REL               = 0x09
  TMP               = 0x0A
  BUTTON            = 0x0B
  BUZZER            = 0x0C
  LED_RED           = 0x0D
  LED_GREEN         = 0x0E
  LED_BLUE          = 0x0F
  WIFI_CRED         = 0x10
  ```

<a id="chapter-3"></a>
Examples of using commands
===============================================================================================================================

<a id="chapter-4"></a>
**ECHO**              0x00

Checking the data transmission.
```markdown
  [MAGIC:3][LENGTH:1][ECHO:1][CRC8:1]
```
---------------------------------

<a id="chapter-5"></a>
**ACK**              0x01

After sending any command, the device under test sends a response with a description of the state of the device, various parameters and flags.
```markdown
  [MAGIC:3][LENGTH:1][ACK:1][ATE401State:sizeoff(ATE401State)][CRC8:1]
```

[ATE401State structure ](#chapter-23)
  - Version
  - Time
  - TXD
  - RXD
  - OUT
  - RTE
  - DC
  - REL
  - TMP
  - Capacitive Button
  - Buzzer
  - Led Red 
  - Led Green
  - Led Blue
  - IP Address 
---------------------------------

<a id="chapter-6"></a>
**TEST_MODE**         0x02

Puts the device in test mode.
```markdown
  [MAGIC:3][LENGTH:1][TEST_MODE:1][ATE_PARAM:1][CRC8:1]
```
* ATE_PARAM
    * OFF = 0
    * ON = 1
---------------------------------

<a id="chapter-7"></a>
**SET_TIME**         0x03

Used [UnixTime](https://en.wikipedia.org/wiki/Unix_time)

```markdown
  [MAGIC:3][LENGTH:1][SET_TIME:1][TIME:4|Little-Endian][CRC8:1]
```

```c++
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

<a id="chapter-8"></a>
**TXD**         0x04

Read byte.

To check the TXD line in ip401 EEPROM sets 1 byte using test equipment (I2C), and then reads this byte using the UART. TFor checking bytes are compared.
```markdown
  [MAGIC:3][LENGTH:1][TXD:1][CRC8:1]
```
---------------------------------

<a id="chapter-9"></a>
**RXD**         0x05

Set byte.

To test the RXD line in ip401 EEPROM sets 1 byte via UART, and then this byte is read through the I2C interface. For checking bytes are compared.
```markdown
  [MAGIC:3][LENGTH:1][RXD:1][PARAM:1][CRC8:1]
```
* PARAM
    * 1 char
---------------------------------

<a id="chapter-10"></a>
**OUT**         0x06

Set pin state.

OUT pin sets to LOW or HIGH state, then test equipment takes measurements.
```markdown
  [MAGIC:3][LENGTH:1][OUT:1][STATE:1][CRC8:1]
```
* STATE
    * OFF = 0
    * ON = 1
---------------------------------

<a id="chapter-11"></a>
**RTE**         0x07

Read pin state.

RTE pin sets to LOW or HIGH via the test equipment, then ip401 reads the input measurement result.
```markdown
  [MAGIC:3][LENGTH:1][RTE:1][CRC8:1]
```
---------------------------------

<a id="chapter-12"></a>
**DC**         0x08

Read pin state.

DC pin sets to LOW or HIGH via the test equipment, then ip401 reads the input measurement result.
```markdown
  [MAGIC:3][LENGTH:1][DC:1][CRC8:1]
```
---------------------------------

<a id="chapter-13"></a>
**REL**         0x09

Set pin state.

REL pin sets to LOW or HIGH state, then test equipment takes measurements.
```markdown
  [MAGIC:3][LENGTH:1][REL:1][STATE:1][CRC8:1]
```
* STATE
    * OFF = 0
    * ON = 1
---------------------------------

<a id="chapter-14"></a>
**TMP**         0x0A

Read pin state.

TMP pin sets to LOW or HIGH via the test equipment, then ip401 reads the input measurement result.
```markdown
  [MAGIC:3][LENGTH:1][TMP:1][CRC8:1]
```
---------------------------------

<a id="chapter-15"></a>
**BUTTON**         0x0B

Read pin state.

BUTTON pin sets to LOW or HIGH via the test equipment, then ip401 reads the input measurement result.
```markdown
  [MAGIC:3][LENGTH:1][BUTTON:1][CRC8:1]
```
---------------------------------

<a id="chapter-16"></a>
**BUZZER**           0x0C

Set pin state.

BUZZER pin sets state, then test equipment takes measurements.
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

<a id="chapter-17"></a>
**LED_RED**           0x0D

Set pin state.

LED_RED pin sets to LOW or HIGH state, then test equipment takes measurements.
```markdown
  [MAGIC:3][LENGTH:1][LED_RED:1][STATE:1][CRC8:1]
```
* STATE
    * OFF = 0
    * ON = 1
---------------------------------

<a id="chapter-18"></a>
**LED_GREEN**         0x0E

Set pin state.

LED_RED pin sets to LOW or HIGH state, then test equipment takes measurements.
```markdown
  [MAGIC:3][LENGTH:1][LED_GREEN:1][STATE:1][CRC8:1]
```
* STATE
    * OFF = 0
    * ON = 1
---------------------------------

<a id="chapter-19"></a>
**LED_BLUE**         0x0F

Set pin state.

LED_RED pin sets to LOW or HIGH state, then test equipment takes measurements.
```markdown
  [MAGIC:3][LENGTH:1][LED_BLUE:1][STATE:1][CRC8:1]
```
* STATE
    * OFF = 0
    * ON = 1
---------------------------------

<a id="chapter-20"></a>
**WIFI_CRED**         0x10

Set Wi-Fi credential.

If the connection is successful, ip401 returns the IP address.
```markdown
  [WIFI_CRED:1][SSID:N][NTS][PASSWORD:M][NTS]
```
  * NTS - 0x00 "null terminated string"
---------------------------------

<a id="chapter-21"></a>
Reply
===============================================================================================================================
Upon successful delivery and reading of the command, the testing equipment provides a response comand ACK indicating either success or failure.

<a id="chapter-24"></a>
The data will come in the form of a structure:
```c++
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
```

<a id="chapter-22"></a>
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
  TP20 - Capacitive Button (send command on/off via I2c, read value via UART)
  TP21 - JTDO for programming
  TP22 - TDI for JTAG (not used) 

  LED - RED and GREEN (send command on/off via I2c, read value via UART)
```

<a id="chapter-23"></a>
Optional structures
===============================================================================================================================

```c++
  enum class PiATE401Cmd : uint8_t
  {
    ECHO       = 0x00,
    ACK        = 0x01,
    TEST_MODE  = 0x02,
    SET_TIME   = 0x03,
    TXD        = 0x04,
    RXD        = 0x05,
    OUT        = 0x06,
    RTE        = 0x07,
    DC         = 0x08,
    REL        = 0x09,
    TMP        = 0x0A,
    BUTTON     = 0x0B,
    BUZZER     = 0x0C,
    LED_RED    = 0x0D,
    LED_GREEN  = 0x0E,
    LED_BLUE   = 0x0F,
    WIFI_CRED  = 0x10,
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