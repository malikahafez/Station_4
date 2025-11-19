#ifndef RFID_CONFIG_H
#define RFID_CONFIG_H

#include "hardware/spi.h"

// Define the SPI instance you are using (GPIO 4, 5, 6, 7 map to SPI0)
#define RFID_SPI_INSTANCE spi0 

// --- Map your D-pins to the Pico's physical GPIO numbers ---

// SPI Signals
#define RFID_SCK_GPIO_PIN     6  // D13 / GPIO 6
#define RFID_COPI_GPIO_PIN    7  // D11 / GPIO 7 (MOSI)
#define RFID_CIPO_GPIO_PIN    4  // D12 / GPIO 4 (MISO)

// Control Signals (GPIO pins used for Chip Select and Reset)
#define RFID_SS_GPIO_PIN      5  // D10 / GPIO 5 (Chip Select/Slave Select)
#define RFID_RST_GPIO_PIN     21 // D9  / GPIO 21 (Reset)

#endif // RFID_CONFIG_H
