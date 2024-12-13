
# Smart Plant Irrigation System

A smart irrigation system using ESP32, capacitive soil moisture sensors, relays, water pumps, and the Blynk IoT platform for real-time monitoring and control.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Setup](#software-setup)
- [Installation](#installation)
- [Circuit Diagram](#circuit-diagram)
- [BLYNK App Dashboard](#blynk-app-dashboard)
- [Usage](#usage)
- [License](#license)

## Introduction
This project automates plant watering using soil moisture data. It integrates with the Blynk app, allowing users to monitor sensor data and control water pumps remotely.

## Features
- Automatic watering based on soil moisture levels.
- Real-time monitoring of soil moisture, temperature, and humidity.
- Real-time data for dry soil.
- Remote control for draining tank.
- Remote control and configuration via the Blynk app.

## Hardware Requirements
- ESP32 microcontroller
- Capacitive soil moisture sensors (x3)
- Water pumps (x4)
- Relay modules (x4)
- DHT20 temperature and humidity sensor
- Power source
- Jumper wires

## Software Setup
1. Arduino IDE with the following libraries:
   - Blynk
   - DHT sensor library
   - WiFi library
2. Blynk app installed on a smartphone.

## Installation
1. Clone this repository.
2. Open the code in the Arduino IDE.
3. Install the necessary libraries.
4. Configure the Wi-Fi credentials and Blynk authentication token in the code.
5. Connect the components as per the circuit diagram.
6. Upload the code to the ESP32 board.

## Circuit Diagram
![AquaSense-SmartPlantIrrigationSystem](https://github.com/user-attachments/assets/40b9853b-7af5-49ff-b5f7-f6815fa172c0)

## BLYNK App Dashboard
![IMG_2622](https://github.com/user-attachments/assets/17cd61fb-1cad-4e84-9ea3-021ee6103197)



## Usage
1. Power on the ESP32 board.
2. Open the Blynk app and monitor sensor data.
3. Configure soil moisture thresholds to automate watering.
4. Use the app to manually control water pumps if needed.

## License
This project is licensed under the MIT License. See [LICENSE](https://github.com/idhanwal/Smart-Plant-Irrigation-System?tab=GPL-3.0-1-ov-file#) for details.
