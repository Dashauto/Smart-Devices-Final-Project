# ESE-5190 Smart Devices Final Project
Final Repository for ESE-5190 Smart Devices

## Overview
This repository contains the final project for ESE-5190. It is about a smart curtain opener and includes various components like a clock, environmental sensors, servo motor control, and more, all integrated with an UNO R3 and an ESP32.

## Components

### 1. Clock
- **Timing**: 0:00 - 11:59
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

Beacause of time emergency, we do not have enough time to do more test, as a result, we need to embrace this fact. That means we need to use only 4 byte of data to create a very short and efficient communication protocol.

There are 13 instructions below we need for this task:

- A. Open curtain
- B. Close curtain
- C. Toggle condition of air-conditioner
- E. Set time to open curtain
- F. Set time to close curtain
- G. Set temperature to open air-conditioner
- H. set temperature to close air-conditioner
- I. set current time
- J. Smart decision swith for curtain
- K. Smart decision swith for air-conditioner
- X. Current time 
- Y. Current temperature and humidity 

Since the way we describe protocol is by 4 Bytes of data, eg, `buffer[4]`, the first one shall be instruction identifier, and the last one shall be ending identifier.

In this case, we only have 2 Bytes to transmit data. Luckily, 2 Bytes are enough for our purpose.

First, for instructions A, B, C, J, K, these 5 instructions only transmit on/off signal, don't need to include data part.

Then, for instructions G, H, Y, these 3 instructions need to send temperature and humidty data
- The second entry of the transmission buffer, eg, `buffer[2]` will include 

