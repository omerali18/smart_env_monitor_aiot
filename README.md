# Smart Environmental Monitor (ESP32-C3 AIoT Project)

A personal AIoT project built using the ESP32-C3 and ESP-IDF 5.4. This system continuously monitors environmental conditions, detects anomalies, and integrates cloud features like OTA and remote provisioning using ESP RainMaker.

---

## Features

- Real-time Temperature & Humidity monitoring (DHT11)
- OLED display using SSD1306
- Buzzer alerts on anomalies
- AI-based anomaly detection using TinyML (in ESP-IDF version)
- ESP RainMaker provisioning & cloud control
- OTA update support

---

## Hardware Setup

| Component        | Details                            |
|------------------|------------------------------------|
| Microcontroller  | ESP32-C3 (XIAO ESP32-C3)            |
| Sensor           | DHT11 (GPIO2)                      |
| Display          | SSD1306 OLED 128x32 (I2C GPIO6/7)  |
| Buzzer           | GPIO4                              |
| Push Button      | GPIO5                              |
| Environment      | ESP-IDF v5.4                        |

---

## Getting Started

### 1. Clone the Project

### 2. Set up ESP-IDF and Configure

idf.py set-target esp32c3 idf.py menuconfig # optional tweaks idf.py build idf.py -p [PORT] flash monitor


---

## ESP RainMaker

- Use the ESP RainMaker mobile app for provisioning via BLE.
- Monitor sensor data, toggle alerts, and receive OTA updates.

---

## AI-Based Detection (ESP-IDF Only)

- TinyML module runs inference on real-time temperature/humidity.
- Alerts are triggered on deviations from normal patterns.

---

## OTA Firmware Updates

- OTA is enabled through ESP RainMaker.
- Upload new firmware via app or dashboard when needed.

---

## Screenshots / Media (To Add)

- OLED screen displaying live values  
- RainMaker dashboard status  
- Hardware prototype images  

---

## Folder Structure

smart_env_monitor_aiot/
├── main/
│   ├── app_main.c
│   ├── app_wifi.c/.h
│   ├── sensor_task.c/.h
│   ├── oled_display.c/.h
│   ├── gpio_control.c/.h
│   ├── ota_update.c/.h
│   ├── tinyml_ai.c/.h
│   └── idf_component.yml
├── components/
│   ├── esp-idf-ssd1306/
│   └── esp32-dht/
├── CMakeLists.txt
├── sdkconfig
└── README.md

---

##Author

**Mohammed Omer Ali**  
Master's Student,  
Embedded & AIoT Student

---

## License

This project is for personal academic use. If reused or referenced, please credit appropriately.

---
#   s m a r t _ e n v _ m o n i t o r _ a i o t  
 