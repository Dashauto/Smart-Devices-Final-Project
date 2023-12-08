# ESE-5190 Smart Devices Final Project
Final Repository for ESE-5190 Smart Devices

## Overview
This repository contains the final project for ESE-5190. It is about a smart curtain opener and includes various components like a clock, environmental sensors, servo motor control, and more, all integrated with an UNO R3 and an ESP32.

## Components

### 1. Clock
- **Timing**: 0:00 - 23:59
- **Features**:
  - `timer1` for precise timing.
  - Integration with ESP32 for network time synchronization (TBD).

### 2. Environmental Sensor
- **Functionality**: Continuously monitors and compares environmental data against set thresholds.
- **Sensors**:
  - Temperature & Humidity:
    - Input: `D2 (PD2)`
  - Photoresistor:
    - Type: ADC
    - Input: `A0 (PC0)`

### 3. Servo Motor Control
- **Mode**: Timer0 phase-correct mode
- **Output**: `PD6`

### 4. Fan/Air Conditioner Control
- Controls DC motor for fan/air conditioner operations.

- DC motor
  - '+' : `PD3`
  - '-' : `GND`

### 5. Communication with ESP32
- Details of communication protocols and methods with ESP32.

#### 5.1 UART
- **Mode**: 2 stop bits, 8 data bits, no parity bits
- Atmega Pin:
  - TX - `TX`
  - RX - `RX`
  - GND - `GND`

- ESP32 Pin:  
  - TX - `GPIO 7`
  - RX - `GPIO 8`
  - GND - `GND`
  
#### 5.2 SPI
- Master: Atmega 328P

- Atmega Pin:
  - SCK - `13`
  - MISO - `12`
  - MOSI - `11`
  - CS / SS - `10`

- Slave: ESP32 (use ESP32SPISLAVE library)

- ESP32 Pin:
  - SCK - `GPIO 5`
  - MISO - `GPIO 21`
  - MOSI - `GPIO 19`
  - CS / SS - `GPIO 33`

### 6. ESP32 & Blynk Integration
- Integration details with Blynk app for remote monitoring and control.

## Self-Designed Communication Protocol
After a series of designs and tests, we found out that Atmega328P can only receive 4 Bytes of data at once under interrupt mode. eg, `buffer[4]`

Beacause of time constrain, we do not have enough time to do more test, as a result, we need to embrace this fact. That means we need to use only 4 byte of data to create a very short and efficient communication protocol.

### Protocol Structure
- **Buffer Size:** 4 Bytes (`buffer[4]`)
- **Buffer Composition:**
  - `buffer[0]`: Instruction Identifier
  - `buffer[1]`: Data Byte 1 (if needed)
  - `buffer[2]`: Data Byte 2 (if needed)
  - `buffer[3]`: Ending Identifier

### Instructions Set (13 Total)
1. **A:** Open curtain
2. **B:** Close curtain
3. **C:** Toggle air-conditioner
4. **D:** Not used
5. **E:** Set time to open curtain
6. **F:** Set time to close curtain
7. **G:** Set temperature to open air-conditioner
8. **H:** Set temperature to close air-conditioner
9. **I:** Set current time
10. **J:** Smart decision switch for curtain
11. **K:** Smart decision switch for air-conditioner
12. **X:** Current time
13. **Y:** Current temperature and humidity
14. **Z:** Not Used

### Instruction Categories
- **On/Off Signal Instructions (A, B, C, J, K):** Do not require additional data.
- **Temperature and Humidity Instructions (G, H, Y):**
  - `buffer[2]`: Humidity data (int to char + 32 for visibility)
  - `buffer[3]`: Temperature data (int to char + 32 for visibility)
- **Time Transmit Instructions (E, F, I, X):**
  - `buffer[2]`: Hour data (int to char + 32 for visibility)
  - `buffer[3]`: Minute data (int to char + 32 for visibility)

### Data Representation
- Data in `int` format is converted to `char + 32`. This is for debugging purposes, as ASCII codes become visible starting from 32.




