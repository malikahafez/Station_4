<!-- # Room 1 Station 4
## Drivers and Test
### Driver folders for the following sensors and actuators:
* LCD 16x2
* UV LED 
* Potentiometer 10kohm
* Push Button
* RFID Reader
* Water Pump
* Water Sensor
### Main 
Tests that the drivers are working -->

# Station 4 - Embedded Systems Project

Multi-sensor and actuator control system built on Arduino Nano (RP2040) Connect.

## Prerequisites

1. **Raspberry Pi Pico SDK**: Version 2.2.0 or later
2. **CMake**: Version 3.13 or later
3. **VS Code** with Raspberry Pi Pico extension
4. **Picotool**: Version 2.2.0-a4 (for flashing)


## Setup Instructions

### 1. Install Pico SDK

The Pico SDK should be installed at `%USERPROFILE%\.pico-sdk\`. If not installed, follow the [official Pico SDK installation guide](https://github.com/raspberrypi/pico-sdk).

### 2. Build the Project

1. Open the project folder in VS Code
2. Press `Ctrl+Shift+B` or run the "Compile Project" task
3. The build output will be generated in the `build/` directory
4. The compiled `.uf2` file will be at `build/Station_4.uf2`

### 3. Flash to Arduino Nano RP2040 Connect

1. Connect the Arduino Nano RP2040 Connect to your computer via USB
2. Connect the RESET and GND pins on the Arduino Nano RP2040 Connect using a male to male wire
3. Click the **BOOTSEL** button 
4. Remove the male to male wire
5. The board will appear as a USB drive
6. Copy `build/Station_4.uf2` to the USB drive
7. The Arduino Nano RP2040 will automatically reboot with the new firmware

_Check out the [Arduino Nano RP2040 Connect documentation](https://docs.arduino.cc/micropython/board-examples/nano-rp2040-connect/) for reference:_

## Running the Project

Once flashed, the system will start automatically. The LCD will display system status and sensor readings.

### OCR Server

OCR validation:

1. Install Python dependencies:
   ```bash
   pip install -r requirements.txt
   ```

2. Run the OCR server:
   ```bash
   python3 local_ocr_test.py
   ```

