# 🌱 Smart Irrigation and Environmental Monitoring System using ESP32

# Live Simulation

Wokwi:
https://wokwi.com/projects/467195007656427521

Click Run to simulate the Smart Irrigation System.

## Source Code

GitHub Repository:
https://github.com/eager-31/smart-irrigation-system-esp32

## Screenshots

### Circuit Diagram
![Circuit Diagram](images/circuit_diagram.png)

### Pump ON Condition
![Pump ON](images/pump_on.png)


Overview
An IoT-based Smart Irrigation and Environmental Monitoring System built with the ESP32 microcontroller, DHT22 sensor, and a soil moisture sensor. The system continuously monitors soil and environmental conditions, and automatically triggers irrigation when soil moisture drops below a predefined threshold — eliminating the need for manual intervention.
---
Features
🌡️ Real-time temperature monitoring via DHT22
💧 Real-time humidity monitoring via DHT22
🌾 Continuous soil moisture sensing
⚙️ Automatic pump ON/OFF control based on moisture threshold
🔴 LED indicator for pump status
📟 Live sensor output via Serial Monitor
🖥️ Wokwi simulation support for hardware-free testing
---
Components Used
Component	Description
ESP32	Main microcontroller
DHT22	Temperature & Humidity sensor
Soil Moisture Sensor	Simulated using a potentiometer in Wokwi
LED	Visual pump ON/OFF indicator
---
System Workflow
```
Start
  │
  ├── Read Soil Moisture (ADC)
  ├── Read Temperature & Humidity (DHT22)
  │
  ├── Soil Moisture < Threshold?
  │       ├── YES → Turn Pump ON  (LED ON)
  │       └── NO  → Keep Pump OFF (LED OFF)
  │
  └── Display readings on Serial Monitor → Repeat
```
---
Working
ESP32 reads the soil moisture level via ADC.
ESP32 reads temperature and humidity from the DHT22 sensor.
If soil moisture is below the threshold:
Pump turns ON (LED lights up as indicator).
Otherwise:
Pump remains OFF.
All sensor data is continuously printed to the Serial Monitor.
---
Technologies Used
Embedded C++ — firmware logic
ESP32 — microcontroller with built-in ADC and Wi-Fi
ADC (Analog-to-Digital Conversion) — for soil moisture reading
IoT — sensor-driven automated control
Wokwi — browser-based simulation environment
---
Project Structure
```
smart-irrigation-esp32/
├── src/
│   └── main.cpp          # Core firmware logic
├── diagram.json          # Wokwi circuit diagram
├── wokwi.toml            # Wokwi project config
└── README.md
```
---
Future Improvements
📡 MQTT Integration — publish sensor data to a broker
📱 Blynk Dashboard — mobile-based monitoring and control
☁️ Cloud Data Logging — store historical sensor readings
🔔 Mobile Notifications — alert on critical moisture levels
🔌 Relay-Controlled Water Pump — real hardware pump integration
---
Author
Deepak Birajee  
B.Tech, Electronics and Communication Engineering  
IIIT Bhagalpur
