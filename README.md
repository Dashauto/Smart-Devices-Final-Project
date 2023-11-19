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

### 6. ESP32 & Blynk Integration
- Integration details with Blynk app for remote monitoring and control.

## Getting Started
(Include instructions on how to set up and run the project.)

