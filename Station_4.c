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
// #include <stdio.h>
// #include <stdbool.h>
// #include "pico/stdlib.h"
// #include "RFID_Reader_Driver.h"

// int main() {
//     stdio_init_all();
//     sleep_ms(2000);
//     rfid_init();

//     printf("RFID Reader Test Started.\n");

//     RFID_State rfid_state;
//     rfid_driver_init(&rfid_state);

//     while (true) {
//         const char* uid = rfid_driver_poll(&rfid_state);
//         if (uid) {
//             printf("Card detected! UID = %s\n", uid);
//             printf("UID returned by driver: %s\n", uid);
//         }

//         sleep_ms(200); // small polling delay
//     }
// }

// FINAL TEST - ALL DRIVERS TOGETHER - PASS
// #include <stdio.h>
// #include <stdbool.h>
// #include "pico/stdlib.h"

// // Include all your drivers
// #include "LCD_Driver.h"
// #include "Potentiometer_Driver.h"
// #include "Push_Button_Driver.h"
// #include "Water_Sensor_Driver.h"
// #include "Water_Pump_Driver.h"
// #include "LED_Driver.h"
// #include "RFID_Reader_Driver.h"

// // --- Thresholds / Constants ---
// #define MOVEMENT_THRESHOLD 1
// #define LOOP_DELAY_MS 100   // Main loop delay (adjust for polling speed)

// // --- Main ---
// int main() {
//     stdio_init_all();
//     sleep_ms(1000);
//     printf("Pico Multi-Driver Test Started.\n");

//     // --- Initialize Drivers ---
//     lcd_init();
//     LED_init();
//     potentiometer_init();
//     button_init();
//     water_sensor_init();
//     water_pump_init();
//     rfid_init();

//     printf("All drivers initialized.\n");

//     // Initialize RFID state
//     RFID_State rfid_state;
//     rfid_driver_init(&rfid_state);

//     // Variables for pot tracking
//     int prev_pot = read_potentiometer_mapped(0, 100);

//     // Pump state
//     bool pump_on = false;

//     // LCD initial message
//     lcd_set_cursor(0, 0);
//     lcd_print("System Test");
//     lcd_set_cursor(0, 1);
//     lcd_print("Pot: 0% | W: 0%");

//     while (true) {
//         // ------------------------------
//         // 1. Potentiometer
//         // ------------------------------
//         int pot_percent = read_potentiometer_mapped(0, 100);
//         int diff = pot_percent - prev_pot;
//         if (diff > MOVEMENT_THRESHOLD) {
//             printf("CW (value increasing)\n");
//             prev_pot = pot_percent;
//         } else if (diff < -MOVEMENT_THRESHOLD) {
//             printf("CCW (value decreasing)\n");
//             prev_pot = pot_percent;
//         }

//         // ------------------------------
//         // 2. Water Sensor
//         // ------------------------------
//         int water_percent = read_water_percent();
//         if (water_percent > 50) {
//             printf("-> Water Detected! %d%%\n", water_percent);
//         }

//         // ------------------------------
//         // 3. Push Button
//         // ------------------------------
//         if (was_button_just_pressed()) {
//             pump_on = !pump_on; // Toggle pump
//             if (pump_on) {
//                 water_pump_set_direction(true); // Forward
//                 printf("Button pressed. Pump ON.\n");
//             } else {
//                 water_pump_off();
//                 printf("Button pressed. Pump OFF.\n");
//             }
//             LED_toggle(); // Optional indicator
//         }

//         // ------------------------------
//         // 4. Water Pump Control
//         // ------------------------------
//         if (pump_on) {
//             water_pump_set_speed(pot_percent);
//         } else {
//             water_pump_set_speed(0);
//         }

//         // ------------------------------
//         // 5. LCD Update
//         // ------------------------------
//         lcd_set_cursor(0, 1);
//         lcd_print("Pot:");
//         lcd_print_number(pot_percent);
//         lcd_print("% W:");
//         lcd_print_number(water_percent);
//         lcd_print("%   "); // Clear extra chars

//         // ------------------------------
//         // 6. RFID Card Polling
//         // ------------------------------
//         const char* uid = rfid_driver_poll(&rfid_state);
//         if (uid) {
//             printf("Card detected! UID = %s\n", uid);
//             lcd_set_cursor(0, 0);
//             lcd_print(uid);
//         }

//         // ------------------------------
//         // 7. LED Blinking Example
//         // ------------------------------
//         // Optional: blink LED slowly if pump is off
//         static uint32_t last_led_toggle = 0;
//         if (!pump_on) {
//             uint32_t now = to_ms_since_boot(get_absolute_time());
//             if (now - last_led_toggle > 500) { // toggle every 500 ms
//                 LED_toggle();
//                 last_led_toggle = now;
//             }
//         }

//         // ------------------------------
//         // 8. Loop Delay
//         // ------------------------------
//         sleep_ms(LOOP_DELAY_MS);
//     }

//     return 0;
// }
// #include <stdio.h>
// #include <stdbool.h>
// #include "pico/stdlib.h"

// // Drivers
// #include "LCD_Driver.h"
// #include "Potentiometer_Driver.h"
// #include "Push_Button_Driver.h"
// #include "Water_Sensor_Driver.h"
// #include "Water_Pump_Driver.h"
// #include "LED_Driver.h"
// #include "RFID_Reader_Driver.h"

// int main() {
//     stdio_init_all();
//     sleep_ms(1500);
//     printf("=== Combined Hardware Test Started ===\n\n");

//     // ---------- Init Drivers ----------
//     lcd_init();
//     printf("LCD Init OK\n");
//     lcd_set_cursor(0, 0);
//     lcd_print("LCD OK");
//     sleep_ms(1000);
//     send_command(0x01);
//     sleep_ms(2);

//     potentiometer_init();
//     printf("Potentiometer Init OK\n");
//     lcd_set_cursor(0, 0);
//     lcd_print("Potentiometer OK");
//     sleep_ms(1000);
//     send_command(0x01);
//     sleep_ms(2);

//     button_init();
//     printf("Button Init OK\n");
//     lcd_set_cursor(0, 0);
//     lcd_print("Button OK");
//     sleep_ms(1000);
//     send_command(0x01);
//     sleep_ms(2);

//     water_sensor_init();
//     printf("Water Sensor Init OK\n");
//     lcd_set_cursor(0, 0);
//     lcd_print("Water Sensor OK");
//     sleep_ms(1000);
//     send_command(0x01);
//     sleep_ms(2);

//     water_pump_init();
//     printf("Water Pump Init OK\n");
//     lcd_set_cursor(0, 0);
//     lcd_print("Water Pump OK");
//     sleep_ms(1000);
//     send_command(0x01);
//     sleep_ms(2);

//     LED_init();
//     printf("LED Init OK\n");
//     lcd_set_cursor(0, 0);
//     lcd_print("LED OK");
//     sleep_ms(1000);
//     send_command(0x01);
//     sleep_ms(2);

//     RFID_State rfid_state;
//     rfid_driver_init(&rfid_state);
//     printf("RFID Init OK\n");
//     lcd_set_cursor(0, 0);
//     lcd_print("RFID OK");
//     sleep_ms(1000);
//     send_command(0x01);
//     sleep_ms(2);

//     // ---------- LCD Initial Text ----------
//     lcd_set_cursor(0, 0);
//     lcd_print("Combined Test");

//     int counter = 0;
//     int b = 0;

//     // ---------- Main Loop ----------
//     while (true) {

//         // --------------------------------
//         // Potentiometer
//         // --------------------------------
//         int pot_percent = read_potentiometer_mapped(0, 100);
//         printf("Potentiometer: %d%%\n", pot_percent);

//         // --------------------------------
//         // Button
//         // --------------------------------
//         if (was_button_just_pressed()) {
//             printf("Button Pressed!\n");
//             b++;
//         }

//         // --------------------------------
//         // Water Sensor
//         // --------------------------------
//         int water_percent = read_water_percent();
//         int water_raw = read_water_raw();
//         printf("Water Sensor: %d%% (raw: %d)\n", water_percent, water_raw);

//         // --------------------------------
//         // RFID
//         // --------------------------------
//         const char* uid = rfid_driver_poll(&rfid_state);
//         if (uid) {
//             printf("RFID Card Detected! UID: %s\n", uid);
//             lcd_set_cursor(0,0);
//             lcd_print(uid);
//         }

//         // --------------------------------
//         // LED Blink
//         // --------------------------------
//         if (counter % 20 == 0) {
//             LED_on();
//             printf("LED ON\n");
//         } else if (counter % 20 == 10) {
//             LED_off();
//             printf("LED OFF\n");
//         }

//         // --------------------------------
//         // Water Pump Demo Cycle
//         // --------------------------------
//         // if (counter % 1000 == 0) {
//         //     printf("Pump: Forward 90%%\n");
//         //     water_pump_set_direction(true);
//         //     water_pump_set_speed(90);
//         // } else if (counter % 1000 == 50) {
//         //     printf("Pump: OFF\n");
//         //     water_pump_off();
//         // }
//         // else{
//         //     printf("Pump: Reverse 100%%\n");
//         //     water_pump_set_direction(false);
//         //     water_pump_set_speed(100);
//         // }

//         // --------------------------------
//         // LCD Output
//         // --------------------------------
//         lcd_set_cursor(0, 1);
//         lcd_print(" Pot:");
//         lcd_print_number(pot_percent);
//         lcd_print("%");
//         lcd_print(" W:");
//         lcd_print_number(water_percent);
//         lcd_print("%");
//         lcd_print("b:");
//         lcd_print_number(b);

//         counter++;
//         sleep_ms(100);
//     }

//     return 0;
// }

//***FINAL TEST FOR VIDEO***
#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "LCD_Driver.h"
#include "Potentiometer_Driver.h"
#include "Push_Button_Driver.h"
#include "Water_Sensor_Driver.h"
#include "Water_Pump_Driver.h"
#include "LED_Driver.h"
#include "RFID_Reader_Driver.h"

int main() {
    stdio_init_all();
    sleep_ms(2000);
//     // ---------- Init Drivers ----------
    lcd_init();
    printf("LCD Init OK\n");
    lcd_set_cursor(0, 0);
    lcd_print("LCD OK");
    sleep_ms(1000);
    send_command(0x01);
    sleep_ms(2);

    potentiometer_init();
    printf("Potentiometer Init OK\n");
    lcd_set_cursor(0, 0);
    lcd_print("Potentiometer OK");
    sleep_ms(1000);
    send_command(0x01);
    sleep_ms(2);

    button_init();
    printf("Button Init OK\n");
    lcd_set_cursor(0, 0);
    lcd_print("Button OK");
    sleep_ms(1000);
    send_command(0x01);
    sleep_ms(2);

    water_sensor_init();
    printf("Water Sensor Init OK\n");
    lcd_set_cursor(0, 0);
    lcd_print("Water Sensor OK");
    sleep_ms(1000);
    send_command(0x01);
    sleep_ms(2);

    water_pump_init();
    printf("Water Pump Init OK\n");
    lcd_set_cursor(0, 0);
    lcd_print("Water Pump OK");
    sleep_ms(1000);
    send_command(0x01);
    sleep_ms(2);

    LED_init();
    printf("LED Init OK\n");
    lcd_set_cursor(0, 0);
    lcd_print("LED OK");
    sleep_ms(1000);
    send_command(0x01);
    sleep_ms(2);

    rfid_init();
    RFID_State rfid_state;
    rfid_driver_init(&rfid_state);
    printf("RFID Init OK\n");
    lcd_set_cursor(0, 0);
    lcd_print("RFID OK");
    sleep_ms(1000);
    send_command(0x01);
    sleep_ms(2);

    // ---------- LCD Initial Text ----------
    lcd_set_cursor(0, 0);
    lcd_print("Test");
    lcd_set_cursor(0, 1);
    lcd_print("Pot: 0% | W: 0%");

    int b = 0;
    int counter = 0;
    while (true) {
        //RFID Reader
        const char* uid = rfid_driver_poll(&rfid_state);
        if (uid) {
            printf("Card detected! UID = %s\n", uid);
            printf("UID returned by driver: %s\n", uid);
            lcd_set_cursor(0,0);
            lcd_print(uid);
        }
        // Button
        if (was_button_just_pressed()) {
            printf("Button Pressed!\n");
            b++;
            sleep_ms(250);  
        }
        // LED Blink
        if (counter % 20 == 0) {
            LED_on();
            printf("LED ON\n");
        } else if (counter % 20 == 10) {
            LED_off();
            printf("LED OFF\n");
        }
        // Potentiometer
        int pot_percent = read_potentiometer_mapped(0, 100);
        printf("Potentiometer: %d%%\n", pot_percent);
        // Water Sensor
        int water_percent = read_water_percent();
        int water_raw = read_water_raw();
        printf("Water Sensor: %d%% (raw: %d)\n", water_percent, water_raw);
        //Water Pump
        water_pump_set_speed(pot_percent);
        if(b % 2 == 0){
            water_pump_set_direction(false);
        }
        else{
            water_pump_set_direction(true);
        }
        // LCD Output
        lcd_set_cursor(0, 1);
        lcd_print("Pot:");
        lcd_print_number(pot_percent);
        lcd_print("% W:");
        lcd_print_number(water_percent);
        lcd_print("% ");
        // lcd_set_cursor(0, 0);
        // lcd_print("Test b:");
        // lcd_print_number(b);
        // lcd_print("   "); 
        counter++;
        sleep_ms(50); // small polling delay
    }
}

