# Protocol ATE401 I2C 'ATE' <-> 'IP401' | 19.03.2024

IPL License

Copyright 2024 U-Prox

Permission is hereby granted, to the employees of U-Prox company.

* [Overview](#chapter-0)
* [Transport part](#chapter-1)
* [Command part](#chapter-2)
* [Examples of using commands](#chapter-3)
  * [ACK](#chapter-4)
  * [TXD](#chapter-5)
  * [RTE](#chapter-6)
  * [DC](#chapter-7)
  * [TMP](#chapter-8)
  * [BUTTON](#chapter-9)
  
* [Reply](#chapter-10)
* [Test points](#chapter-11)
* [Optional structures](#chapter-12)

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
  [MAGIC:3]               | magic bytes '#@!'
  [LENGTH:1]              | length includes data size, LENGTH and CRC8 (without magic bytes)
  [PAYLOAD:N]             | data N bytes
  [CRC8:1]                | CRC8 from LENGTH to CRC8
```

<a id="chapter-2"></a>
Commands part
===============================================================================================================================

```
  ACK               = 0x00
  TXD               = 0x01
  RTE               = 0x02
  DC                = 0x03
  TMP               = 0x04
  BUTTON            = 0x05
  ```

<a id="chapter-3"></a>
Examples of using commands
===============================================================================================================================

<a id="chapter-4"></a>
**ACK**              0x00

After sending any command, the device under test sends a response with a description of the state of the device, various parameters and flags.
```markdown
  [MAGIC:3][LENGTH:1][ACK:1][ATE401State:sizeoff(ATE401State)][CRC8:1]

  ex:
  ['#', '@', '!', 3 + sizeof(ATE401State) /*LENGTH*/, 0x00 /*ACK*/, [ATE401State : sizeof(ATE401State)], crc8]
```

[ATE401State structure ](#chapter-100)
  - Version
  - RXD
  - OUT
  - REL
  - Buzzer
  - Led Red 
  - Led Green
  - Led Blue
  - Voltage TP3
  - Voltage TP13
  - Voltage TP18

    *voltage TP3, TP13, TP18 - measured automatically
---------------------------------

<a id="chapter-5"></a>
**TXD**         0x01

Read byte.

To check the TXD line in ip401 EEPROM sets 1 byte using test equipment (I2C), and then reads this byte using the UART. TFor checking bytes are compared.
```markdown
  [MAGIC:3][LENGTH:1][TXD:1][CRC8:1]

  ex:
  TXD ON
  ['#', '@', '!', 4 /*LENGTH*/, 0x01 /*TXD*/, 1 /*STATE*/, crc8]

  TXD OFF
  ['#', '@', '!', 4 /*LENGTH*/, 0x01 /*TXD*/, 0 /*STATE*/, crc8]
```
---------------------------------

<a id="chapter-6"></a>
**RTE**         0x02

Read pin state.

RTE pin sets to LOW or HIGH via the test equipment, then ip401 reads the input measurement result.
```markdown
  [MAGIC:3][LENGTH:1][RTE:1][CRC8:1]

  ex:
  RTE ON
  ['#', '@', '!', 4 /*LENGTH*/, 0x02 /*RTE*/, 1 /*STATE*/, crc8]
  RTE OFF
  ['#', '@', '!', 4 /*LENGTH*/, 0x02 /*RTE*/, 0 /*STATE*/, crc8]
```
---------------------------------

<a id="chapter-7"></a>
**DC**         0x03

Read pin state.

DC pin sets to LOW or HIGH via the test equipment, then ip401 reads the input measurement result.
```markdown
  [MAGIC:3][LENGTH:1][DC:1][CRC8:1]

  ex:
  DC ON
  ['#', '@', '!', 4 /*LENGTH*/, 0x03 /*DC*/, 1 /*STATE*/, crc8]

  DC OFF
  ['#', '@', '!', 4 /*LENGTH*/, 0x03 /*DC*/, 0 /*STATE*/, crc8]
```
---------------------------------

<a id="chapter-8"></a>
**TMP**         0x04

Read pin state.

TMP pin sets to LOW or HIGH via the test equipment, then ip401 reads the input measurement result.
```markdown
  [MAGIC:3][LENGTH:1][TMP:1][CRC8:1]

  ex:
  TMP ON
  ['#', '@', '!', 4 /*LENGTH*/, 0x04 /*TMP*/, 1 /*STATE*/, crc8]

  TMP OFF
  ['#', '@', '!', 4 /*LENGTH*/, 0x04 /*TMP*/, 0 /*STATE*/, crc8]
```
---------------------------------

<a id="chapter-9"></a>
**BUTTON**         0x05

Read pin state.

BUTTON pin sets to LOW or HIGH via the test equipment, then ip401 reads the input measurement result.
```markdown
  [MAGIC:3][LENGTH:1][BUTTON:1][CRC8:1]

  ex:
  ON
  ['#', '@', '!', 4 /*LENGTH*/, 0x05 /*BUTTON*/, 1 /*STATE*/, crc8]

  OFF
  ['#', '@', '!', 4 /*LENGTH*/, 0x05 /*BUTTON*/, 0 /*STATE*/, crc8]
```
---------------------------------

<a id="chapter-10"></a>
Read data
===============================================================================================================================
If the data in the EEPROM has been updated, then you can read it in the following format.

<a id="chapter-100"></a>
The data will come in the form of a structure:
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
    float voltage_TP3;
    float voltage_TP13;
    float voltage_TP18;
  };
```

<a id="chapter-11"></a>
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

<a id="chapter-12"></a>
Optional structures
===============================================================================================================================

```c++
  enum class ATE401Cmd : uint8_t
  {
    ACK        = 0x00,
    TXD        = 0x01,
    RTE        = 0x02,
    DC         = 0x03,
    TMP        = 0x04,
    BUTTON     = 0x05,
  };
```
```c++
  enum class ATE401Param : uint8_t
  {
    OFF = 0,
    ON = 1,
  };
```
