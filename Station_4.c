// #include <stdio.h>
// #include <stdbool.h>
// #include "pico/stdlib.h"

// #include "LCD_Driver.h"//working
// #include "Potentiometer_Driver.h"//working - check pins
// #include "Push_Button_Driver.h"//working
// #include "Water_Sensor_Driver.h"//needs testing - check pins
// #include "Water_Pump_Driver.h"//working - check input pins
// #include "LED_Driver.h"//working
// #include "RFID_Reader_Driver.h"//needs testing
// #include "utils.h" 

// // How much change is required to treat as movement (tune for stability)
// #define MOVEMENT_THRESHOLD 1


// int main() {
//     // Initialize standard I/O (USB Serial) for printf debugging
//     stdio_init_all(); 
//     sleep_ms(1000); // Wait for serial connection to stabilize
//     printf("Pico Multi-Driver Test Started.\n");

//     // Initialize all drivers
//     lcd_init();
//     LED_init();
//     potentiometer_init();
//     button_init();
//     water_sensor_init();
//     water_pump_init();
//     rfid_init();
//     printf("Drivers init complete.\n");

//     lcd_set_cursor(0, 0);
//     lcd_print("System Test");
//     lcd_set_cursor(0, 1);
//     lcd_print("Pot: 0% | W: 0%");

//     int pot_percent = 0;
//     int water_percent = 0;
//     bool pump_on = false;

//     bool waiting_for_removal = false;

//     uint16_t prev = read_potentiometer_mapped(0, 100); 
//     sleep_ms(10);

//     while (true) {
//         // --- 1. Read Inputs (Potentiometer and Water Sensor) ---
//         // Read the potentiometer value mapped from 0% to 100%
//         pot_percent = read_potentiometer_mapped(0, 100); 
//         int diff = pot_percent - prev;

//         if (diff > MOVEMENT_THRESHOLD) {
//             printf("CW (value increasing)\n");
//             prev = pot_percent;     // Update for next comparison
//         }
//         else if (diff < -MOVEMENT_THRESHOLD) {
//             printf("CCW (value decreasing)\n");
//             prev = pot_percent;
//         }

//         // Read the water sensor value mapped to 0% to 100% saturation
//         water_percent = read_water_percent();


//         // --- 2. Process Logic (Button Press controls Pump) ---
//         if (was_button_just_pressed()) {
//             pump_on = !pump_on; // Toggle the pump state
            
//             if (pump_on) {
//                 // Set pump to forward direction (assuming 'true' is forward)
//                 water_pump_set_direction(true); 
//                 printf("Button pressed. Pump ON.\n");
//             } else {
//                 water_pump_off(); // Stop the pump
//                 printf("Button pressed. Pump OFF.\n");
//             }
//             LED_toggle(); // Optional: Add LED indication for pump state change
//         }


//         // --- 3. Control Outputs (Pump Speed and LCD Display) ---
//         if (pump_on) {
//             // Set the pump speed based on the potentiometer setting
//             water_pump_set_speed(pot_percent); 
//         } else {
//             // Ensure pump is off if pump_on is false.
//             // water_pump_off() is called when pump_on is toggled off, 
//             // but this ensures it stays off if the logic is changed.
//             // You can remove this 'else' block if water_pump_set_speed(0) 
//             // is equivalent to water_pump_off().
//             water_pump_set_speed(0);
//         }

//         // Update the LCD display with current readings
//         lcd_set_cursor(0, 1);
//         lcd_print("Pot:");
//         lcd_print_number(pot_percent);
//         lcd_print("% W:");
//         lcd_print_number(water_percent);
//         lcd_print("% "); // Add spaces to clear previous text


//         // --- 4. Debug Output (Optional) ---
//         printf("Pot: %d%% | Water: %d%% | Pump Status: %s\n", 
//                 pot_percent, water_percent, pump_on ? "ON" : "OFF");
        
//         // If a card is detected
//         if (rfid_is_card_present()) {

//             if (!waiting_for_removal) {
//                 const char* uid = rfid_read_card_uid();

//                 if (uid) {
//                     printf("Card detected! UID = %s\n", uid);
//                     lcd_set_cursor(0,0);
//                     lcd_print(uid);
//                 } else {
//                     printf("Card detected but UID read failed!\n");
//                 }

//                 // Prevent spamming until card is removed
//                 waiting_for_removal = true;
//             }

//             // sleep_ms(200);  // debounce
//         }
//         else {
//             if (waiting_for_removal) {
//                 printf("Card removed.\n");
//             }
//             waiting_for_removal = false;

//             // sleep_ms(100);
//         }
//         // Debounce/Loop delay
//         sleep_ms(200); 
//     }

//     return 0;
// }
//LCD TEST - PASS
// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "LCD_Driver.h"

// int main() {
//     stdio_init_all();
//     sleep_ms(1000);
//     printf("LCD Driver Test Started.\n");

//     lcd_init();
//     printf("LCD Init Complete.\n");

//     lcd_set_cursor(0, 0);
//     lcd_print("LCD Driver Test");
    
//     int counter = 0;
//     while (true) {
//         lcd_set_cursor(0, 1);
//         lcd_print("Count: ");
//         lcd_print_number(counter);
//         lcd_print("    "); // Clear remaining space
        
//         printf("Displaying count: %d\n", counter);
//         counter++;
//         sleep_ms(500);
//     }
//     return 0;
// }
//Potwntiometer Test - PASS *check why stops at 99%
// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "Potentiometer_Driver.h"

// int main() {
//     stdio_init_all();
//     sleep_ms(1000);
//     printf("Potentiometer Driver Test Started.\n");

//     potentiometer_init();
//     printf("Potentiometer Init Complete.\n");

//     while (true) {
//         // Read the potentiometer value mapped from 0% to 100%
//         int pot_percent = read_potentiometer_mapped(0, 100); 
        
//         printf("Potentiometer Value: %d%%\n", pot_percent);
//         sleep_ms(100);
//     }
//     return 0;
// }
//Push Button Test - PASS
// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "Push_Button_Driver.h"

// int main() {
//     stdio_init_all();
//     sleep_ms(1000);
//     printf("Push Button Driver Test Started.\n");

//     button_init();
//     printf("Button Init Complete.\n");

//     while (true) {
//         if (was_button_just_pressed()) {
//             printf("--- Button Just Pressed! ---\n");
//         }
//         sleep_ms(50); // Small delay to avoid busy-waiting too hard
//     }
//     return 0;
// }

//Water Sensor Test - PASS
// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "Water_Sensor_Driver.h"

// int main() {
//     stdio_init_all();
//     sleep_ms(1000);
//     printf("Water Sensor Driver Test Started.\n");

//     water_sensor_init();
//     printf("Water Sensor Init Complete. Make sure to check pins.\n");

//     while (true) {
//         // Read the water sensor value mapped to 0% to 100% saturation
//         int water_percent = read_water_percent();

//         printf("Water Saturation: %d%%\n", water_percent);
//         printf("Actual Value: %d\n", read_water_raw());
        
//         // This threshold will depend on your sensor calibration
//         if (water_percent > 50) {
//             printf("-> Water Detected!\n");
//         } else {
//             printf("-> Sensor Dry.\n");
//         }
//         sleep_ms(500);
//     }
//     return 0;
// }

//water pump test - PASS
// #include <stdio.h>
// #include <stdbool.h>
// #include "pico/stdlib.h"
// #include "Water_Pump_Driver.h"

// int main() {
//     stdio_init_all();
//     sleep_ms(1000);
//     printf("Water Pump Driver Test Started.\n");

//     water_pump_init();
//     printf("Water Pump Init Complete. Check input pins.\n");
    
//     // Cycle through speed and direction changes
//     while (true) {
//         // Test 1: Forward at 50% speed
//         printf("Test 1: Forward at 50%%\n");
//         water_pump_set_direction(true); // Forward
//         water_pump_set_speed(50);
//         sleep_ms(2000);

//         // Test 2: Stop
//         printf("Test 2: Stopping pump\n");
//         water_pump_off();
//         sleep_ms(1000);

//         // Test 3: Reverse at 100% speed
//         printf("Test 3: Reverse at 100%%\n");
//         water_pump_set_direction(false); // Reverse
//         water_pump_set_speed(100);
//         sleep_ms(2000);

//         // Test 4: Stop
//         printf("Test 4: Stopping pump\n");
//         water_pump_set_speed(0); // Another way to stop
//         sleep_ms(1000);
//     }
//     return 0;
// }

//LED test - PASS
// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "LED_Driver.h"

// int main() {
//     stdio_init_all();
//     sleep_ms(1000);
//     printf("LED Driver Test Started.\n");

//     LED_init();
//     printf("LED Init Complete.\n");

//     while (true) {
//         printf("LED ON\n");
//         LED_on();
//         sleep_ms(500);

//         printf("LED OFF\n");
//         LED_off();
//         sleep_ms(500);
//     }
//     return 0;
// }

//RFID Reader test - PASS
#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "RFID_Reader_Driver.h"

int main() {
    stdio_init_all();
    sleep_ms(2000);
    rfid_init();

    printf("RFID Reader Test Started.\n");

    RFID_State rfid_state;
    rfid_driver_init(&rfid_state);

    while (true) {
        const char* uid = rfid_driver_poll(&rfid_state);
        if (uid) {
            printf("Card detected! UID = %s\n", uid);
            printf("UID returned by driver: %s\n", uid);
        }

        sleep_ms(200); // small polling delay
    }
}


