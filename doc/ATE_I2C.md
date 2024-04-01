# Protocol ATE401 I2C 'ATE' <-> 'IP401' | 19.03.2024

IPL License

Copyright 2024 U-Prox

Permission is hereby granted, to the employees of U-Prox company.

* [Overview](#chapter-0)
* [Transport part](#chapter-1)
* [Memory area part](#chapter-2)
  * [Read](#chapter-3)
  * [Write](#chapter-4)
* [Examples of using memmory area](#chapter-5)
* [Test points](#chapter-6)
* [Optional structures](#chapter-87)

<a id="chapter-0"></a>
Overview
===============================================================================================================================
The data exchange I2C protocol between Raspberry Pi and test equipment ATE. This protocol facilitates seamless communication and data transfer. It specifies the format, structure, and rules for transmitting and receiving data, allowing the Raspberry Pi and the test equipment to understand and interpret the information exchanged between them accurately.

![ate](/doc/ate.png)

<a id="chapter-1"></a>
Transport part
===============================================================================================================================
```markdown
Packets struct:
  [MEMORY AREA:1]         | EEPROM memory area
  [PAYLOAD:N]             | data N bytes
```

<a id="chapter-2"></a>
Memory area part
===============================================================================================================================
EEPROM is divided into 2 memory areas, 
- **0x0000** - area for reading the status of pins
- **0x0100**  - area of setting the state of pins

<a id="chapter-3"></a>
Reading pins status area start from **0x0000**
```
VERSION_HI         0x0000
VERSION_LOW        0x0001

RXD                0x0002
OUTPUT             0x0003
RELAY              0x0004
BUZZER             0x0005

LED_RED_HI         0x0006
LED_RED_LOW        0x0007

LED_GREEN_HI       0x0008
LED_GREEN_LOW      0x0009

LED_BLUE_HIGH      0x000A
LED_BLUE_LOW       0x000B

VOLTAGE_TP3_HI     0x000C
VOLTAGE_TP3_LOW    0x000D

VOLTAGE_TP13_HI    0x000E
VOLTAGE_TP13_LOW   0x000F

VOLTAGE_TP18_HI    0x0010
VOLTAGE_TP18_LOW   0x0011
```
<a id="chapter-4"></a>
Writing pins status area start from **0x0100**
```
TXD              0x0100
RTE              0x0101
DC               0x0102
TMP              0x0103
BUTTON           0x0104
CMD_STROB        0x0105

```

<a id="chapter-6"></a>
Examples of using memmory area
===============================================================================================================================

```markdown
  [MEMORY AREA:1][SIZE:N]
```
```
  Read
  [0x0000 /*VERSION*/, 2 /* LE LENGTH*/]
  [0x0002 /*RXD*/, 1 /*LENGTH*/]
  [0x0003 /*OUTPUT*/, 1 /*LENGTH*/]
  [0x0004 /*RELAY*/, 1 /*LENGTH*/]
  [0x0005 /*BUZZER*/, 1 /*LENGTH*/]
  [0x0007 /*LED_RED*/, 2 / LE *LENGTH*/]
  [0x0009 /*LED_GREEN*/, 2 /* LE LENGTH*/]
  [0x0011 /*LED_BLUE*/, 2 /* LE LENGTH*/]
  [0x0013 /*VOLTAGE_TP3*/, 2 /* LE LENGTH*/]
  [0x0015 /*VOLTAGE_TP13*/, 2 /* LE LENGTH*/]
  [0x0017 /*VOLTAGE_TP18*/, 2 /* LE LENGTH*/]
  [0x0200 /*START_TIME_CALIBRATION*/, 1 /*LENGTH*/]
```
```
  Write
  [0x0100 /*TXD*/, 1 /*LENGTH*/]
  [0x0101 /*RTE*/, 1 /*LENGTH*/]
  [0x0102 /*DC*/, 1 /*LENGTH*/]
  [0x0103 /*TMP*/, 1 /*LENGTH*/]
  [0x0104 /*BUTTON*/, 1 /*LENGTH*/]
  [0x0105 /*CMD_STROB*/, 1 /*LENGTH*/]
```
<a id="chapter-7"></a>
Test points
===============================================================================================================================
The value of the ip401 gpio outputs is measured by the test equipment, after which the data can be transferred via I2C to the Raspberry Pi.

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

  LED - RED, GREEN and BLUE (send command on/off via UART, read value via I2C)
```

<a id="chapter-8"></a>
Optional structures
===============================================================================================================================

```c++
  enum class EEPROM_READ_ADDR : uint8_t
  {
    VERSION_HI       = 0x0000,
    VERSION_LOW      = 0x0001,
    RXD              = 0x0002,
    OUTPUT           = 0x0003,
    RELAY            = 0x0004,
    BUZZER           = 0x0005,
    LED_RED_HI       = 0x0006,
    LED_RED_LOW      = 0x0007,
    LED_GREEN_HI     = 0x0008,
    LED_GREEN_LOW    = 0x0009,
    LED_BLUE_HI      = 0x000A,
    LED_BLUE_LOW     = 0x000B,
    VOLTAGE_TP3_HI   = 0x000C,
    VOLTAGE_TP3_LOW  = 0x000D,
    VOLTAGE_TP13_HI  = 0x000E,
    VOLTAGE_TP13_LOW = 0x000F,
    VOLTAGE_TP18_HI  = 0x0010,
    VOLTAGE_TP18_LOW = 0x0011,
  };
```

```c++
  enum class EEPROM_WRITE_ADDR : uint8_t
  {
    TXD              = 0x0100,
    RTE              = 0x0101,
    DC               = 0x0102,
    TMP              = 0x0103,
    BUTTON           = 0x0104,
    CMD_STROB        = 0x0105,
  };
```

```c++
struct ATE401_i2c {
    uint16_t version;
    uint8_t rxd;
    uint8_t output;
    uint8_t rellay;
    uint8_t buzzer;
    uint8_t led_red;
    uint8_t led_green;
    uint8_t led_blue;
    uint16_t voltage_TP3;
    uint16_t voltage_TP13;
    uint16_t voltage_TP18;
  };
```
