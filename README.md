# Arduino Data Logger & Telemetry System

A practical embedded system built on the Arduino UNO platform for real-time monitoring and logging of analog voltages and ambient temperature.

## Features
* **Multi-Channel Monitoring:** Simultaneously reads 3 voltage channels (0-5V) and calculates temperature using an NTC thermistor.
* **Dynamic User Interface:** Features 5 distinct display modes on an I2C LCD (16x2), sequentially toggled via a hardware-debounced push button.
* **Non-Volatile Data Logging:** A long press/click triggers an immediate save of all current parameters directly to the Arduino's internal EEPROM, protected against power loss.
* **Real-Time Clock Synchronization:** Each log entry is timestamped using an external DS1307 RTC module.
* **Live Telemetry:** Streams purely numeric data packets via the Serial port, fully optimized for the Arduino IDE Serial Plotter without interface disruptions.

## Hardware Components
* Arduino UNO Rev3
* LCD Display 16x2
* RTC DS1307 Module
* 1x NTC Thermistor (10kΩ)
* 3x Potentiometers
* 1x Push Button

## How to Run and Test
1. **Circuit Assembly:** Connect the components according to the pinout diagram.
2. **Library Requirements:** Ensure you have installed the following libraries in your Arduino IDE:
   * `LiquidCrystal_I2C`
   * `RTClib`
3. **Upload:** Compile and upload the sketch to your Arduino board.
4. **Live Graphing:** Open the **Serial Plotter** 