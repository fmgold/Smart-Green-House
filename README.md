# 🌱 Smart Greenhouse Monitoring System

This is an **IoT-based Smart Greenhouse Monitoring System** built using an **ESP32** microcontroller, a **DHT11 sensor**, **soil moisture sensor**, **LDR (light sensor)**, and various actuators like a fan, light, and water pump. The system displays real-time environmental data on an I2C LCD and syncs it with a **Firebase Realtime Database** via Wi-Fi. It also allows remote automation control through Firebase.

---

## 📌 Features

- 🌡️ **Temperature & Humidity Monitoring** (via DHT11)  
- 💧 **Soil Moisture Monitoring**  
- ☀️ **Ambient Light Monitoring** (via LDR)  
- 🔁 **Automatic Actuation**:
  - Fan control for high temperature  
  - Light control for low ambient light  
  - Pump control for low soil moisture  
- 🔗 **Firebase Integration**:
  - Real-time monitoring and control  
  - Automatic mode toggle (`/automatic` path in Firebase)  
- 📟 **LCD Display (I2C 20x4)**:
  - Displays sensor data and system status  
- 🪛 **Manual/Automatic Control Support** through Firebase commands

---

## 🧰 Hardware Requirements

- ESP32 development board  
- DHT11 sensor  
- Soil Moisture Sensor  
- LDR (Light Dependent Resistor)  
- Relay Module (to control fan, light, pump)  
- I2C 20x4 LCD Display  
- 5V/3.3V Power Supply  
- Wires, Breadboard  

---

## 🔌 Pin Configuration

| Component           | ESP32 Pin        |
|---------------------|------------------|
| DHT11               | GPIO 14          |
| Soil Moisture       | GPIO 33 (Analog) |
| LDR                 | GPIO 34 (Analog) |
| Fan (Relay Output)  | GPIO 15          |
| Light (Relay Output)| GPIO 18          |
| Pump (Relay Output) | GPIO 19          |
| LCD (I2C)           | SDA - 21, SCL - 22 |

---

## 🔗 Firebase Setup

1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Create a new project.
3. Go to **Realtime Database** → Create Database → Start in test mode.
4. Copy your **Database URL** and **Web API Key**.
5. Replace in the code:

   ```cpp
   #define FIREBASE_HOST "your_project.firebaseio.com"
   #define FIREBASE_AUTH "your_firebase_database_secret"
