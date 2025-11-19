#include "rfid_reader_driver.h"

#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <string.h>

#include "mfrc522.h"

static MFRC522Ptr_t mfrc = NULL;
static char uid_hex_string[32];

void rfid_init(void) {

    // --- HARD RESET FIRST ---
    gpio_init(RESET_PIN);
    gpio_set_dir(RESET_PIN, GPIO_OUT);

    gpio_put(RESET_PIN, 0);
    sleep_ms(50);
    gpio_put(RESET_PIN, 1);
    sleep_ms(50);

    // --- THEN CONFIGURE SPI ---
    spi_init(spi0, MFRC522_BIT_RATE);  // 4 MHz

    gpio_set_function(sck_pin,  GPIO_FUNC_SPI);
    gpio_set_function(mosi_pin, GPIO_FUNC_SPI);
    gpio_set_function(miso_pin, GPIO_FUNC_SPI);

    // --- CS PIN ---
    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, 1);

    sleep_ms(10);

    // --- ALLOCATE OBJECT ---
    mfrc = MFRC522_Init();
    if (!mfrc) {
        printf("[RFID] ERROR: MFRC522_Init returned NULL!\n");
        return;
    }

    // --- ASSIGN STRUCT MEMBERS ---
    mfrc->spi = spi0;
    mfrc->_chipSelectPin = cs_pin;

    // --- INITIALIZE MFRC522 ---
    PCD_Init(mfrc, spi0);

    // --- ANTENNA ON ---
    PCD_AntennaOn(mfrc);

    printf("[RFID] MFRC522 initialized successfully.\n");
}


bool rfid_is_card_present(void) {
    if (!mfrc) return false;
    return PICC_IsNewCardPresent(mfrc);
}

const char* rfid_read_card_uid() {
    if (!mfrc) return NULL;

    if (!PICC_ReadCardSerial(mfrc)) {
        printf("PICC_ReadCardSerial() failed inside read_card_uid\n");
        return NULL;
    }

    memset(uid_hex_string, 0, sizeof(uid_hex_string));
    char *p = uid_hex_string;

    for (int i = 0; i < mfrc->uid.size; i++) {
        if (i > 0) *p++ = ':';
        p += snprintf(p, sizeof(uid_hex_string) - (p - uid_hex_string), "%02X", mfrc->uid.uidByte[i]);
    }

    // DO NOT halt here
    // PICC_HaltA(mfrc);

    return uid_hex_string;
}


bool rfid_debug_read_raw() {
    return PICC_ReadCardSerial(mfrc);
}

// This function blocks until a card is detected, reads its UID, 
// waits for the card to be removed, and then returns the UID string.
const char* rfid_wait_for_card_once() {
    static char uid_buffer[32]; // persists outside function
    static bool card_present = false;

    while (true) {
        bool present = rfid_is_card_present();

        if (present && !card_present) {
            // First detection of a new card
            if (!PICC_ReadCardSerial(mfrc)) {
                printf("PICC_ReadCardSerial() failed!\n");
                sleep_ms(200); // retry delay
                continue;
            }

            // Format UID
            memset(uid_buffer, 0, sizeof(uid_buffer));
            char* p = uid_buffer;
            for (int i = 0; i < mfrc->uid.size; i++) {
                if (i > 0) *p++ = ':';
                p += snprintf(p, sizeof(uid_buffer) - (p - uid_buffer), "%02X", mfrc->uid.uidByte[i]);
            }

            printf("Card detected! UID = %s\n", uid_buffer);
            card_present = true;

            // Wait until card is removed
            while (rfid_is_card_present()) {
                sleep_ms(200);
            }

            PICC_HaltA(mfrc);
            printf("Card removed.\n");
            card_present = false;

            // Return UID once
            return uid_buffer;
        }

        sleep_ms(50); // small polling delay
    }
}
void rfid_driver_init(RFID_State* state) {
    state->card_present = false;
    memset(state->uid_buffer, 0, sizeof(state->uid_buffer));
}

/**
 * Call this in your main loop.
 * Returns pointer to UID string if a new card is detected,
 * or NULL if no new card.
 */
const char* rfid_driver_poll(RFID_State* state) {
    bool present = rfid_is_card_present();

    if (present && !state->card_present) {
        // First detection of a new card
        if (!PICC_ReadCardSerial(mfrc)) {
            return NULL; // failed to read UID, try next loop
        }

        // Format UID into string
        char* p = state->uid_buffer;
        for (int i = 0; i < mfrc->uid.size; i++) {
            if (i > 0) *p++ = ':';
            p += snprintf(p, sizeof(state->uid_buffer) - (p - state->uid_buffer), "%02X", mfrc->uid.uidByte[i]);
        }

        state->card_present = true;
        return state->uid_buffer; // return UID once
    }

    if (!present && state->card_present) {
        // Card was removed
        PICC_HaltA(mfrc);
        state->card_present = false;
    }

    return NULL; // no new card
}

