#ifndef RFID_READER_DRIVER_H
#define RFID_READER_DRIVER_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "mfrc522.h"    // 3rd-party MFRC522 library

// State for the driver
typedef struct {
    bool card_present;    // true if card currently on reader
    char uid_buffer[32];  // formatted UID string
} RFID_State;

// ----------------------
//  SPI & PIN Definitions
// ----------------------

#define RFID_SPI_PORT           spi0

#define RFID_MOSI_GPIO_PIN      7   // GPIO 7  (SPI0 MOSI)
#define RFID_MISO_GPIO_PIN      4   // GPIO 4  (SPI0 MISO)
#define RFID_SCK_GPIO_PIN       6   // GPIO 6  (SPI0 SCK)

#define RFID_CS_GPIO_PIN        5   // GPIO 5 (CS)
#define RFID_RST_GPIO_PIN       21  // GPIO 21 (RST)

// ----------------------
//  Public API
// ----------------------

void rfid_init(void);
bool rfid_is_card_present(void);
const char* rfid_read_card_uid(void);
bool rfid_debug_read_raw(void);
const char* rfid_wait_for_card_once();

void rfid_driver_init(RFID_State* state);
const char* rfid_driver_poll(RFID_State* state);

#endif // RFID_READER_DRIVER_H
