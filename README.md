# Water-Quality-Detection-using-IOT

# ESP32 Water Quality Monitoring System

This project is an ESP32-based water quality monitoring system that measures Total Dissolved Solids (TDS), pH, temperature, and turbidity using various sensors. It also features an LCD display for real-time readings and a web-based dashboard for remote monitoring.

## Features
- **WiFi Connectivity**: Connects to a WiFi network for remote monitoring.
- **LCD Display**: Shows real-time sensor readings.
- **Web Dashboard**: Displays sensor data in a clean and user-friendly interface.
- **TDS Measurement**: Reads water's total dissolved solids (mg/L).
- **pH Measurement**: Monitors the pH level of water.
- **Temperature Measurement**: Uses a DS18B20 sensor to read water temperature.
- **Turbidity Measurement**: Checks water clarity.

## Components Used
- **ESP32**: Microcontroller for handling sensor data and hosting the web server.
- **LiquidCrystal I2C LCD (16x2)**: For displaying sensor readings.
- **DS18B20**: Temperature sensor.
- **Turbidity Sensor**: Measures water clarity.
- **pH Sensor**: Reads water pH levels.
- **TDS Sensor**: Measures total dissolved solids in water.
- **WiFi Module (built-in ESP32)**: Enables remote monitoring.

## Circuit Connections
| Component  | ESP32 Pin |
|------------|-----------|
| TDS Sensor | A0        |
| pH Sensor  | 35        |
| Turbidity Sensor | 32  |
| DS18B20 Temperature Sensor | 23 |
| LCD I2C | SDA (21), SCL (22) |

## Installation & Setup
1. **Install Dependencies**: Ensure you have the required Arduino libraries installed:
   - `WiFi.h`
   - `WebServer.h`
   - `LiquidCrystal_I2C.h`
   - `OneWire.h`
   - `DallasTemperature.h`
2. **Upload Code**:
   - Open the provided `.ino` file in Arduino IDE.
   - Set your WiFi credentials (`SSID` and `PASSWORD`).
   - Select the ESP32 board and upload the code.
3. **Connect to WiFi**:
   - After uploading, check the Serial Monitor for the ESP32's IP address.
4. **Access Web Dashboard**:
   - Open a web browser and enter the ESP32's IP address to view sensor data.

## Web Dashboard
- The system hosts a simple web page displaying real-time sensor values.
- Auto-refresh updates data every 2 seconds.

## Usage
- View readings on the LCD screen.
- Access sensor data remotely via the web interface.
- Monitor water quality in real-time.

## Future Improvements
- Add data logging to store sensor readings.
- Implement MQTT for IoT integration.
- Enable mobile app connectivity.



