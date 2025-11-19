#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"

#include "LCD_Driver.h"//working
#include "Potentiometer_Driver.h"//working - check pins
#include "Push_Button_Driver.h"//working
#include "Water_Sensor_Driver.h"//needs testing - check pins
#include "Water_Pump_Driver.h"//working - check input pins
#include "LED_Driver.h"//working
#include "RFID_Reader_Driver.h"//needs testing
#include "utils.h" 

// How much change is required to treat as movement (tune for stability)
#define MOVEMENT_THRESHOLD 1


int main() {
    // Initialize standard I/O (USB Serial) for printf debugging
    stdio_init_all(); 
    sleep_ms(1000); // Wait for serial connection to stabilize
    printf("Pico Multi-Driver Test Started.\n");

    // Initialize all drivers
    lcd_init();
    LED_init();
    potentiometer_init();
    button_init();
    water_sensor_init();
    water_pump_init();
    rfid_init();
    printf("Drivers init complete.\n");

    lcd_set_cursor(0, 0);
    lcd_print("System Test");
    lcd_set_cursor(0, 1);
    lcd_print("Pot: 0% | W: 0%");

    int pot_percent = 0;
    int water_percent = 0;
    bool pump_on = false;

    bool waiting_for_removal = false;

    uint16_t prev = adc_read();
    sleep_ms(10);

    while (true) {
        // --- 1. Read Inputs (Potentiometer and Water Sensor) ---
        // Read the potentiometer value mapped from 0% to 100%
        pot_percent = read_potentiometer_mapped(0, 100); 
        int diff = pot_percent - prev;

        if (diff > MOVEMENT_THRESHOLD) {
            printf("CW (value increasing)\n");
            prev = pot_percent;    // Update for next comparison
        }
        else if (diff < -MOVEMENT_THRESHOLD) {
            printf("CCW (value decreasing)\n");
            prev = pot_percent;
        }

        // Read the water sensor value mapped to 0% to 100% saturation
        water_percent = read_water_percent();


        // // --- 2. Process Logic (Button Press controls Pump) ---
        // if (was_button_just_pressed()) {
        //     pump_on = !pump_on; // Toggle the pump state
            
        //     if (pump_on) {
        //         // Set pump to forward direction
        //         water_pump_set_direction(true); 
        //         printf("Button pressed. Pump ON.\n");
        //     } else {
        //         water_pump_off();
        //         printf("Button pressed. Pump OFF.\n");
        //     }
        // }


        // // --- 3. Control Outputs (Pump Speed and LCD Display) ---
        // if (pump_on) {
            // Set the pump speed based on the potentiometer setting
        water_pump_set_speed(pot_percent); 
        // }

        // Update the LCD display with current readings
        lcd_set_cursor(0, 1);
        lcd_print("Pot:");
        lcd_print_number(pot_percent);
        lcd_print("% W:");
        lcd_print_number(water_percent);
        lcd_print("%     "); // Add spaces to clear previous text


        // // --- 4. Debug Output (Optional) ---
        printf("Pot: %d%% | Water: %d%% | Pump Status: %s\n", 
               pot_percent, water_percent, pump_on ? "ON" : "OFF");
        //test button and LED drivers
        if(was_button_just_pressed())
            LED_toggle();
        // If a card is detected
        if (rfid_is_card_present()) {

            if (!waiting_for_removal) {
                const char* uid = rfid_read_card_uid();

                if (uid) {
                    printf("Card detected! UID = %s\n", uid);
                    lcd_set_cursor(0,0);
                    lcd_print(uid);
                } else {
                    printf("Card detected but UID read failed!\n");
                }

                // Prevent spamming until card is removed
                waiting_for_removal = true;
            }

            // sleep_ms(200);  // debounce
        }
        else {
            if (waiting_for_removal) {
                printf("Card removed.\n");
            }
            waiting_for_removal = false;

            // sleep_ms(100);
        }
        // Debounce/Loop delay
        sleep_ms(200); 
    }

    return 0;
}

// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "rfid_reader_driver.h"

// int main() {
//     stdio_init_all();
//     sleep_ms(2000);   // Give USB serial time to attach

//     printf("\n=== RFID Reader - RC522 (Benjamin Modica) ===\n");
//     printf("Initializing RFID module...\n");

//     rfid_init();
//     printf("RFID init complete.\n");

//     bool waiting_for_removal = false;

//     while (1) {
//         // If a card is detected
//         if (rfid_is_card_present()) {

//             if (!waiting_for_removal) {
//                 const char* uid = rfid_read_card_uid();

//                 if (uid) {
//                     printf("Card detected! UID = %s\n", uid);
//                 } else {
//                     printf("Card detected but UID read failed!\n");
//                 }

//                 // Prevent spamming until card is removed
//                 waiting_for_removal = true;
//             }

//             sleep_ms(200);  // debounce
//         }
//         else {
//             if (waiting_for_removal) {
//                 printf("Card removed.\n");
//             }
//             waiting_for_removal = false;

//             sleep_ms(100);
//         }
//     }
// }
