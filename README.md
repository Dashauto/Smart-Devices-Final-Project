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

- OPEN Button(`V0`) on -> GPIO `13` on -> Atmega `PB1`
- CLOSE Button(`V1`) on -> GPIO `12` on -> Atmega `PB0`

## Getting Started
(Include instructions on how to set up and run the project.)

