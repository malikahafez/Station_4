#include "rfid_reader_driver.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

// From Modica's library
#include "mfrc522.h"

static MFRC522Ptr_t mfrc = NULL;
static char uid_hex_string[32];

void rfid_init() {

    // --- Initialize SPI ---
    spi_init(spi0, 4 * 1000 * 1000);  // 4 MHz, library default bit rate

    gpio_set_function(RFID_SCK_GPIO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(RFID_COPI_GPIO_PIN, GPIO_FUNC_SPI);  // MOSI
    gpio_set_function(RFID_CIPO_GPIO_PIN, GPIO_FUNC_SPI);  // MISO

    // --- Chip select pin ---
    gpio_init(RFID_SS_GPIO_PIN);
    gpio_set_dir(RFID_SS_GPIO_PIN, GPIO_OUT);
    gpio_put(RFID_SS_GPIO_PIN, 1);

    // --- Reset pin ---
    gpio_init(RFID_RST_GPIO_PIN);
    gpio_set_dir(RFID_RST_GPIO_PIN, GPIO_OUT);
    gpio_put(RFID_RST_GPIO_PIN, 1);

    sleep_ms(50);

    // --- Allocate MFRC522 object ---
    mfrc = MFRC522_Init();
    if (!mfrc) {
        printf("[RFID] ERROR: MFRC522_Init() returned NULL!\n");
        return;
    }

    // Override the pin values provided by the default library
    mfrc->_chipSelectPin = RFID_SS_GPIO_PIN;
    mfrc->spi = spi0;

    // --- Initialize the reader chip ---
    PCD_Init(mfrc, spi0);
    PCD_Reset(mfrc);
    PCD_AntennaOn(mfrc);

    printf("[RFID] MFRC522 initialized successfully.\n");
}

bool rfid_is_card_present() {
    if (!mfrc) return false;
    return PICC_IsNewCardPresent(mfrc);
}

const char* rfid_read_card_uid() {
    if (!mfrc) return NULL;

    // Check if card is present
    if (!PICC_IsNewCardPresent(mfrc)) return NULL;

    // Attempt to read UID
    if (!PICC_ReadCardSerial(mfrc)) return NULL;

    // Convert UID to a printable hex string
    memset(uid_hex_string, 0, sizeof(uid_hex_string));

    for (int i = 0; i < mfrc->uid.size; i++) {
        char tmp[4];
        snprintf(tmp, sizeof(tmp), "%02X", mfrc->uid.uidByte[i]);
        strcat(uid_hex_string, tmp);
        if (i < mfrc->uid.size - 1)
            strcat(uid_hex_string, ":");
    }

    // Stop PICC so another card can be scanned
    PICC_HaltA(mfrc);

    return uid_hex_string;
}
