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

Multi-sensor and actuator control system built on Raspberry Pi Pico (RP2040) with optional OCR-based validation via Raspberry Pi companion.

## Project Overview

This embedded system integrates multiple sensors and actuators for a comprehensive control station. The project includes:

- **Hardware Controllers**: LCD display, UV LED, potentiometer, push button, RFID reader, water pump, and water sensor
- **Optional OCR System**: Raspberry Pi-based OCR server for text validation (e.g., RFID card text recognition)

## Hardware Components

### Sensors & Actuators
- **LCD Display**: 16x2 character LCD for system status display
- **UV LED**: Ultraviolet LED for sterilization or indication
- **Potentiometer**: 10kΩ rotary potentiometer for analog input
- **Push Button**: Digital input for user interaction
- **RFID Reader**: MFRC522-based card reader
- **Water Pump**: Controllable water pump with speed and direction control
- **Water Sensor**: Capacitive/resistive water level sensor

### Target Board
- **Raspberry Pi Pico / RP2040** (Arduino Nano RP2040 Connect compatible)

## Project Structure

```
Station_4/
├── Station_4.c              # Main application code
├── utils.c/utils.h          # Utility functions
├── CMakeLists.txt           # CMake build configuration
├── pico_sdk_import.cmake    # Pico SDK integration
├── LCD_Driver/              # LCD 16x2 driver
├── LED_Driver/              # UV LED driver
├── Potentiometer_Driver/    # Potentiometer ADC driver
├── Push_Button_Driver/      # Button input driver
├── RFID_Reader_Driver/      # MFRC522 RFID driver
├── Water_Pump_Driver/       # Water pump control driver
├── Water_Sensor_Driver/     # Water sensor driver
├── pi_OCR_Server.py         # Optional: Raspberry Pi OCR server
├── model.py                 # Optional: OCR validation logic
├── local_ocr_test.py        # Optional: Local OCR testing
├── requirements.txt         # Python dependencies for OCR
└── build/                   # Build output directory
```

## Prerequisites

### For Raspberry Pi Pico Development

1. **Raspberry Pi Pico SDK**: Version 2.2.0 or later
2. **CMake**: Version 3.13 or later
3. **ARM Toolchain**: GNU ARM Embedded Toolchain (14_2_Rel1)
4. **Ninja Build**: v1.12.1 or later
5. **Picotool**: Version 2.2.0-a4 (for flashing)
6. **OpenOCD**: Version 0.12.0+dev (optional, for debugging)

### For Optional OCR System (Raspberry Pi)

1. **Python 3.7+**
2. **Tesseract OCR**: Install via package manager
   ```bash
   sudo apt-get install tesseract-ocr
   ```
3. **USB Camera or Pi Camera**

## Setup Instructions

### 1. Install Pico SDK (Windows)

The project expects the Pico SDK to be installed at:
```
%USERPROFILE%\.pico-sdk\
```

If not already installed, follow the [official Pico SDK installation guide](https://github.com/raspberrypi/pico-sdk).

### 2. Clone the Repository

```bash
git clone <repository-url>
cd Station_4
```

### 3. Configure the Project

The project is pre-configured for CMake. Ensure the following paths are set in `CMakeLists.txt`:
- `PICO_BOARD`: `arduino_nano_rp2040_connect` (or your specific board)
- SDK paths are automatically resolved via environment variables

### 4. Build the Project

#### Using VS Code Tasks (Recommended)

1. Open the project folder in VS Code
2. Press `Ctrl+Shift+B` or run "Compile Project" task
3. The build output will be in the `build/` directory

#### Using Command Line

```bash
# Create build directory if it doesn't exist
mkdir build
cd build

# Configure CMake
cmake -G Ninja ..

# Build the project
ninja
```

The compiled `.uf2` file will be located at:
```
build/Station_4.uf2
```

### 5. Flash to Raspberry Pi Pico

#### Method 1: USB Boot Mode (Easiest)

1. Hold the **BOOTSEL** button on your Pico
2. Connect it to your computer via USB
3. Release the button - Pico will appear as a USB drive
4. Copy `build/Station_4.uf2` to the USB drive
5. The Pico will automatically reboot with the new firmware

#### Method 2: Using Picotool (VS Code Task)

1. Connect your Pico in BOOTSEL mode
2. Run the "Run Project" task in VS Code
3. Or manually:
   ```powershell
   %USERPROFILE%\.pico-sdk\picotool\2.2.0-a4\picotool\picotool.exe load build\Station_4.uf2 -fx
   ```

#### Method 3: Using OpenOCD (Debug Probe Required)

1. Connect a debug probe (e.g., another Pico as Picoprobe)
2. Run the "Flash" task in VS Code

## Running the Project

### Pico Application

Once flashed, the Pico will:

1. Initialize all drivers (LCD, sensors, actuators)
2. Display "System Test" on LCD
3. Show potentiometer and water sensor readings
4. Respond to button presses (toggle water pump)
5. Detect RFID cards and display UID
6. Output debug information via USB serial (115200 baud)

### Monitor Serial Output

Connect to the Pico's USB serial port at **115200 baud**:

```powershell
# Using PuTTY, TeraTerm, or VS Code Serial Monitor
# Port: COMx (Windows) or /dev/ttyACM0 (Linux/Mac)
# Baud: 115200
```

### Optional: OCR Server (Raspberry Pi)

If using the OCR validation system:

#### 1. Install Python Dependencies

```bash
pip install -r requirements.txt
```

#### 2. Configure Serial Port

Edit `pi_OCR_Server.py`:
```python
SERIAL_PORT = '/dev/ttyACM0'  # Adjust for your system
BAUD_RATE = 115200
CAMERA_INDEX = 0
```

#### 3. Run the OCR Server

```bash
python3 pi_OCR_Server.py
```

The server will:
- Connect to the Pico via serial
- Capture frames from the camera
- Perform OCR on captured images
- Validate and send results back to the Pico

#### 4. Test OCR Locally (Optional)

```bash
python3 local_ocr_test.py
```

## Usage

### Basic Operations

1. **Adjust Pump Speed**: Rotate the potentiometer (0-100%)
2. **Toggle Pump**: Press the push button (ON/OFF with LED indicator)
3. **Scan RFID Card**: Hold an RFID card near the reader
4. **Monitor Water Level**: Water sensor reading displayed on LCD
5. **View Status**: Check LCD for real-time readings

### Pin Connections

Refer to individual driver header files for specific pin assignments:
- [LCD_Driver.h](LCD_Driver/LCD_Driver.h)
- [LED_Driver.h](LED_Driver/LED_Driver.h)
- [Potentiometer_Driver.h](Potentiometer_Driver/Potentiometer_Driver.h)
- [Push_Button_Driver.h](Push_Button_Driver/Push_Button_Driver.h)
- [RFID_Reader_Driver.h](RFID_Reader_Driver/RFID_Reader_Driver.h)
- [Water_Pump_Driver.h](Water_Pump_Driver/Water_Pump_Driver.h)
- [Water_Sensor_Driver.h](Water_Sensor_Driver/Water_Sensor_Driver.h)

## Troubleshooting

### Build Issues

- **SDK not found**: Ensure `USERPROFILE` environment variable is set and SDK is installed
- **Ninja errors**: Verify Ninja is in your PATH or at `%USERPROFILE%\.pico-sdk\ninja\`
- **Compilation errors**: Check that all driver `.c` files are present and listed in `CMakeLists.txt`

### Flashing Issues

- **Picotool not found**: Install picotool or use USB boot mode instead
- **Device not detected**: Ensure Pico is in BOOTSEL mode (hold button during connection)
- **Permission denied**: On Linux, add user to `dialout` group: `sudo usermod -a -G dialout $USER`

### Runtime Issues

- **LCD not displaying**: Check I2C connections and address (default 0x27)
- **RFID not reading**: Verify SPI connections and power supply
- **Pump not running**: Check PWM pin and motor driver connections
- **Serial not working**: Ensure baud rate is 115200 and correct port is selected

### OCR System Issues

- **Serial connection failed**: Check USB cable and port permissions
- **Camera not found**: Verify camera index and connections
- **OCR accuracy low**: Improve lighting, adjust camera focus, update `model.py` validation

## Development

### Adding New Drivers

1. Create a new folder: `<Driver_Name>_Driver/`
2. Add `.c` and `.h` files with init and control functions
3. Update `CMakeLists.txt` to include the new source files
4. Include the header in `Station_4.c` and call init in `main()`

### Debugging

Use the serial output for runtime debugging:
```c
printf("Debug message: %d\n", value);
```

For hardware debugging with OpenOCD:
1. Connect a debug probe
2. Use VS Code debugging features
3. Set breakpoints and step through code

## License

[Add your license information here]

## Contributors

[Add contributor information here]

## Acknowledgments

- Raspberry Pi Foundation for Pico SDK
- Driver implementations based on manufacturer datasheets