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

//***FINAL TEST FOR DEMO***
// #include <stdio.h>
// #include <stdbool.h>
// #include "pico/stdlib.h"
// #include "LCD_Driver.h"
// #include "Potentiometer_Driver.h"
// #include "Push_Button_Driver.h"
// #include "Water_Sensor_Driver.h"
// #include "Water_Pump_Driver.h"
// #include "LED_Driver.h"
// #include "RFID_Reader_Driver.h"

// int main() {
//     stdio_init_all();
//     sleep_ms(2000);
// //     // ---------- Init Drivers ----------
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

//     rfid_init();
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
//     lcd_print("Test");
//     lcd_set_cursor(0, 1);
//     lcd_print("Pot: 0% | W: 0%");

//     int b = 0;
//     int counter = 0;
//     while (true) {
//         //RFID Reader
//         const char* uid = rfid_driver_poll(&rfid_state);
//         if (uid) {
//             printf("Card detected! UID = %s\n", uid);
//             printf("UID returned by driver: %s\n", uid);
//             lcd_set_cursor(0,0);
//             lcd_print(uid);
//         }
//         // Button
//         if (was_button_just_pressed()) {
//             printf("Button Pressed!\n");
//             b++;
//             sleep_ms(250);  
//         }
//         // LED Blink
//         if (counter % 20 == 0) {
//             LED_on();
//             printf("LED ON\n");
//         } else if (counter % 20 == 10) {
//             LED_off();
//             printf("LED OFF\n");
//         }
//         // Potentiometer
//         int pot_percent = read_potentiometer_mapped(0, 100);
//         printf("Potentiometer: %d%%\n", pot_percent);
//         // Water Sensor
//         int water_percent = read_water_percent();
//         int water_raw = read_water_raw();
//         printf("Water Sensor: %d%% (raw: %d)\n", water_percent, water_raw);
//         //Water Pump
//         water_pump_set_speed(pot_percent);
//         if(b % 2 == 0){
//             water_pump_set_direction(false);
//         }
//         else{
//             water_pump_set_direction(true);
//         }
//         // LCD Output
//         lcd_set_cursor(0, 1);
//         lcd_print("Pot:");
//         lcd_print_number(pot_percent);
//         lcd_print("% W:");
//         lcd_print_number(water_percent);
//         lcd_print("% ");
//         // lcd_set_cursor(0, 0);
//         // lcd_print("Test b:");
//         // lcd_print_number(b);
//         // lcd_print("   "); 
//         counter++;
//         sleep_ms(50); // small polling delay
//     }
// }

// #include <stdio.h>
// #include <stdbool.h>
// #include "pico/stdlib.h"
// #include "LCD_Driver.h"
// #include "Potentiometer_Driver.h"
// #include "Push_Button_Driver.h"
// #include "Water_Sensor_Driver.h"
// #include "Water_Pump_Driver.h"
// #include "LED_Driver.h"
// #include "RFID_Reader_Driver.h"

// #include "hardware/uart.h"

// #define RFID_CLUE_BLOCK 4     // Block 4 is safe (first data block)
// #define TARGET_CLUE_VALUE 750 // The expected value from the RFID tag

// // UART Configuration
// #define UART_ID uart0
// #define BAUD_RATE 115200
// #define UART_TX_PIN 0
// #define UART_RX_PIN 1

// // Buffer for receiving data
// #define BUFF_SIZE 256

// #define MOVEMENT_THRESHOLD 1
// #define WATER_LEVEL_TOLERANCE 0.3 // +/- 0.3% around the target 75%


// // Global State Variables
// bool direction = false; // CW=0, CCW=1
// int pot_percent; // 0 inititially
// int water_percent;

// int submit = 0;
// bool clue_obtained = false; // New state to track if RFID clue is found
// bool puzzle_solved = false; // New state to prevent re-pumping after success
// bool pumped_out_sucessfully = false;

// char text_buffer[BUFF_SIZE];

// // Global MFRC522 pointer from RFID driver
// extern MFRC522Ptr_t mfrc; // Must be declared to use PICC_HaltA

// void uart_init_custom() {
//     // Initialize UART
//     uart_init(UART_ID, BAUD_RATE);
    
//     // Set the TX and RX pins
//     gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
//     gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
//     // Enable UART FIFO
//     uart_set_fifo_enabled(UART_ID, true);
// }

// void send_ocr_request() {
//     const char *command = "START_OCR\n";
//     uart_puts(UART_ID, command);
//     printf("Sent: START_OCR\n");
// }

// bool receive_ocr_response(char *buffer, size_t buffer_size) {
//     uint32_t timeout = 10000; // 10 seconds timeout in ms
//     uint32_t start_time = to_ms_since_boot(get_absolute_time());
//     int index = 0;
    
//     while ((to_ms_since_boot(get_absolute_time()) - start_time) < timeout) {
//         if (uart_is_readable(UART_ID)) {
//             char c = uart_getc(UART_ID);
            
//             if (c == '\n') {
//                 buffer[index] = '\0';
//                 return true;
//             }
            
//             if (index < buffer_size - 1) {
//                 buffer[index++] = c;
//             }
//         }
//         sleep_ms(10);
//     }
    
//     printf("Timeout waiting for OCR response\n");
//     return false;
// }
// void lcd_show_target(int target_level){
//     send_command(LCD_CLEARDISPLAY);
//     lcd_set_cursor(0,0);
//     char buf[16];
//     snprintf(buf, sizeof(buf), "TGT LVL:%d unt", target_level);
//     lcd_print(buf);
//     // lcd_print("TGT LVL:750 unit");
// }
// // ----------------------------
// // RFID Functions - integer 750
// // ----------------------------
// bool rfid_get_clue() {
//     // Use rfid_driver_poll to check for a new card and update rfid_state
//     const char* uid_str = rfid_driver_poll(&rfid_state);

//     if (uid_str) {
//         printf("NEW CARD DETECTED! UID: %s\n", uid_str);
        
//         uint16_t clue_value = 0;
        
//         // Attempt to read the clue block
//         if (rfid_read_uint16(RFID_CLUE_BLOCK, &clue_value, &rfid_state)) {
//             printf("[RFID] Read clue: %u\n", clue_value);
            
//             if(clue_value == TARGET_CLUE_VALUE){
//                 // Only show the target when the correct card is scanned
//                 lcd_show_target(TARGET_CLUE_VALUE); 
//                 return true;
//             }
//         } else {
//             // Print error if read failed (e.g., Auth failed)
//             printf("[RFID] Failed to read clue block %d\n", RFID_CLUE_BLOCK);
//         }
        
//         // Halt the PICC immediately to ensure a clean re-authentication 
//         PICC_HaltA(mfrc); 
//     }
    
//     // If no card or incorrect clue, check if the card is currently present (for puzzle continuation)
//     // The original logic only proceeds if rfid_get_clue() returns true *this cycle*.
//     // However, since the polling and halting is inside, this function now strictly returns 
//     // true only when a NEW card with the correct clue is detected.
    
//     // To match the original flow where the check only happens on detection, 
//     // we return false if no valid clue was found on polling.
//     return false;
// }

// // Retained for initial setup/testing
// bool rfid_write_test_clue() {
//     printf("Writing RFID clue %u to block %d...\n", TARGET_CLUE_VALUE, RFID_CLUE_BLOCK);

//     if (rfid_write_uint16(RFID_CLUE_BLOCK, TARGET_CLUE_VALUE, &rfid_state)) {
//         printf("RFID write OK\n");
//         return true;
//     } else {
//         printf("RFID write FAILED\n");
//         return false;
//     }
// }

// void pump_in(){
//     water_pump_set_direction(true);
//     water_pump_set_speed(pot_percent); 


// }
// void pump_out(){
//     water_pump_set_direction(false);
//     water_pump_set_speed(100-pot_percent); 

// }

// void lcd_update_current(){
//     water_percent = read_water_percent();
//     lcd_set_cursor(0, 1);
//     lcd_print("Water:");
//     lcd_print_number(water_percent);
//     lcd_print("%   "); // Clear extra characters
// }

// void lcd_show_success() {
//     send_command(LCD_CLEARDISPLAY);
//     lcd_set_cursor(0, 0);
//     lcd_print("Correct Level!");
//     lcd_set_cursor(0, 1);
//     lcd_print("Proceed...");
// }

// void perform_ocr(){
//     send_ocr_request();

//     if (receive_ocr_response(text_buffer, BUFF_SIZE)) {
//         sleep_ms(500);
//         LED_off();

//         send_command(LCD_CLEARDISPLAY);
//         lcd_set_cursor(0, 0);
//         lcd_print("Clue:");
//         lcd_set_cursor(0, 1);
//         lcd_print(text_buffer);

//         printf("%s\n", text_buffer);
//         } else {
//             send_command(LCD_CLEARDISPLAY);
//             lcd_print("OCR Timeout!");
//             LED_off();
//         }
// }

// int main() {
//     stdio_init_all();
//     uart_init_custom();
    
//     printf("Station Flow Test\n");
//     sleep_ms(2000);
    
//     // ---------- Init Drivers ----------
//     lcd_init();
//     potentiometer_init();
//     button_init();
//     water_sensor_init();
//     water_pump_init();
//     LED_init();
//     rfid_init();
//     rfid_driver_init(&rfid_state);

//     // Optional: write test clue 750 to RFID block (uncomment for first run)
//     // To use this, you need a card on the reader and must pause/handle 
//     // the card polling loop to ensure it's not active.
//     // rfid_write_test_clue(); 
//     // sleep_ms(2000);


//     // Variables for pot tracking
//     int prev_pot = read_potentiometer_mapped(0, 100);

//    while (true) {
//         // ----------------------------
//         // Check RFID for the numeric clue
//         // ----------------------------
//         // rfid_get_clue() now handles polling for a NEW card and reading the block.
//         // It returns true only if a NEW card with the correct clue is detected.
//         if (rfid_get_clue()) {

//             // When a NEW card is detected, the puzzle state is 'pumped_out_sucessfully'.
//             if (pumped_out_sucessfully) {
                
//                 // --- POTENTIOMETER CONTROL ---
//                 pot_percent = read_potentiometer_mapped(0, 100);
//                 int diff = pot_percent - prev_pot;

//                 // Stop the pump first
//                 water_pump_set_speed(0);

//                 // Determine rotation direction and pump
//                 if (diff > MOVEMENT_THRESHOLD) {
//                     printf("CW (value increasing)\n");
//                     pump_in();
//                     prev_pot = pot_percent;
//                 } else if (diff < -MOVEMENT_THRESHOLD) {
//                     printf("CCW (value decreasing)\n");
//                     pump_out();
//                     prev_pot = pot_percent;
//                 }
//             }

//             // --- WATER LEVEL & LCD UPDATE ---
//             lcd_update_current();

//             // --- SUBMIT BUTTON CHECK ---
//             if (was_button_just_pressed()) {
//                 printf("Button Pressed!\n");
//                 submit = 1;
//                 sleep_ms(250);

//                 // Check tolerance: 75.0% +/- 0.3%
//                 if (water_percent > 75.3 || water_percent < 74.7) {
//                     // Wrong level - show failure message
//                     send_command(LCD_CLEARDISPLAY);
//                     lcd_set_cursor(0, 0);
//                     lcd_print("Wrong Level!");
//                     lcd_set_cursor(0, 1);
//                     lcd_print("Resetting...");
//                     printf("Water level incorrect: %d%%. Resetting tank.\n", water_percent);
//                     sleep_ms(1000);
                    
//                     // Empty tank to minimum level
//                     // Pump at full speed (100-pot_percent is used by pump_out, but 
//                     // for reset, direct full speed is better)
//                     water_pump_set_direction(false);
//                     water_pump_set_speed(100); 
//                     while (read_water_percent() > 5) { // Pump until near 0%
//                         sleep_ms(50);
//                     }
//                     water_pump_set_speed(0); // Stop pump
                    
//                     pumped_out_sucessfully = true;
//                     submit = 0;
                    
//                     // Show target again after reset
//                     lcd_show_target(TARGET_CLUE_VALUE);
//                 } else {
//                     // Correct level! Show success message
//                     printf("Correct water level achieved: %d%%\n", water_percent);
//                     lcd_show_success();
//                     sleep_ms(1500);
                    
//                     // Turn on UV LED to reveal hidden text
//                     printf("Activating UV LED...\n");
//                     LED_on();
//                     sleep_ms(1000); 
                    
//                     // Scan the revealed text with camera OCR
//                     printf("Starting OCR scan...\n");
//                     perform_ocr();
                    
//                     // Keep the clue displayed
//                     submit = 0;
//                 }
//             }
//         }

//         sleep_ms(50); // Small polling delay
//     }
// }

// #define TEST_VALUE 750

// int main() {
//     stdio_init_all();
//     sleep_ms(1000);

//     printf("RFID / NTAG / MIFARE Test Program\n");

//     rfid_init();
//     rfid_driver_init(&rfid_state);

//     lcd_init();
//     send_command(LCD_CLEARDISPLAY);

//     printf("[MAIN] Waiting for cards...\n");

//     uint8_t target_block = 4; 
//     uint16_t value_to_write = TEST_VALUE;
    
//     while (1) {
//         const char* uid_str = rfid_driver_poll(&rfid_state);

//         if (uid_str) {
//             printf("NEW CARD DETECTED! UID: %s\n", uid_str);
            
//             // --- MIFARE Classic Read/Write ---

//             // --- 1. WRITE OPERATION ---
//             // printf("Attempting to WRITE value %u to block %u...\n", value_to_write, target_block);
//             // if (rfid_write_uint16(target_block, value_to_write, &rfid_state)) {
//             //     printf("Successfully wrote value %u to block %u.\n", value_to_write, target_block);
//             // } else {
//             //     printf("Failed to write value %u to block %u.\n", value_to_write, target_block);
//             // }

//             // // --- CRITICAL FIX: Introduce a pause for card stability ---
//             // // This small pause allows the card to recover from the write command.
//             // sleep_ms(2000); 

//             // --- 2. READ OPERATION ---
//             uint16_t read_value = 0;
//             printf("Attempting to READ value from block %u...\n", target_block);
             
//                 if (rfid_read_uint16(target_block, &read_value, &rfid_state)) {
//                     printf("Successfully read value: %u\n", read_value);
//                 } else {
//                     printf("Failed to read value from block %u.\n");
//                 }


//             // Halt the card to ensure a clean re-detection on the next loop
//             PICC_HaltA(mfrc); 
//         }
//     }
//     return 0;
// }

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

#include "hardware/uart.h"

#define RFID_CLUE_BLOCK 4     // Block 4 is safe (first data block)

// UART Configuration
// #define UART_ID uart0
// #define BAUD_RATE 115200
// #define UART_TX_PIN 0
// #define UART_RX_PIN 1

// Buffer for receiving data
#define BUFF_SIZE 256

#define MOVEMENT_THRESHOLD 1

bool direction = false; // CW=0, CCW=1
int pot_percent; // 0 inititially
int water_percent;

int submit = 0;
bool clue_obtained = false; // New state to track if RFID clue is found
bool puzzle_solved = false; // New state to prevent re-pumping after success
bool pumped_out_sucessfully = false;

char text_buffer[BUFF_SIZE];

// void uart_init_custom() {
//     // Initialize UART
//     uart_init(UART_ID, BAUD_RATE);
    
//     // Set the TX and RX pins
//     gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
//     gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
//     // Enable UART FIFO
//     uart_set_fifo_enabled(UART_ID, true);
// }

void send_ocr_request() {
    const char *command = "START_OCR\n";
    // uart_puts(UART_ID, command);
    printf("%s", command); // Use standard printf for USB Serial
    printf("Sent: START_OCR\n");
}

// bool receive_ocr_response(char *buffer, size_t buffer_size) {
//     uint32_t timeout = 10000; // 10 seconds timeout in ms
//     uint32_t start_time = to_ms_since_boot(get_absolute_time());
//     int index = 0;
    
//     while ((to_ms_since_boot(get_absolute_time()) - start_time) < timeout) {
//         if (uart_is_readable(UART_ID)) {
//             char c = uart_getc(UART_ID);
            
//             if (c == '\n') {
//                 buffer[index] = '\0';
//                 return true;
//             }
            
//             if (index < buffer_size - 1) {
//                 buffer[index++] = c;
//             }
//         }
//         sleep_ms(10);
//     }
    
//     printf("Timeout waiting for OCR response\n");
//     return false;
// }
// Using stdio for USB Serial
bool receive_ocr_response(char *buffer, size_t buffer_size) {
    uint32_t timeout_ms = 10000; // 10 seconds timeout
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    int index = 0;
    
    // Wait for the first character (timeout)
    while (!stdio_usb_connected() && (to_ms_since_boot(get_absolute_time()) - start_time) < 1000) {
        sleep_ms(100);
    }
    
    // Use stdio functions. We read character by character until '\n' or timeout.
    while ((to_ms_since_boot(get_absolute_time()) - start_time) < timeout_ms) {
        // getchar_timeout_us() is a non-blocking read from stdio (USB Serial)
        int c = getchar_timeout_us(0); 

        if (c != PICO_ERROR_TIMEOUT) {
            if (c == '\n' || c == '\r') {
                buffer[index] = '\0';
                return true;
            }
            if (index < buffer_size - 1) {
                buffer[index++] = (char)c;
            }
        }
        sleep_ms(1);
    }
    
    printf("Timeout waiting for OCR response\n");
    return false;
}

void lcd_show_target(int target_level){
    send_command(LCD_CLEARDISPLAY);
    lcd_set_cursor(0,0);
    char buf[16];
    snprintf(buf, sizeof(buf), "TGT LVL:%d unit", target_level);
    lcd_print(buf);
    // lcd_print("TGT LVL:750 unit");
}

// ----------------------------
// RFID Functions - integer 750
// ----------------------------
bool rfid_get_clue() {
    // Use rfid_driver_poll to check for a new card and update rfid_state
    const char* uid_str = rfid_driver_poll(&rfid_state);

    if (uid_str) {
        printf("[RFID] NEW CARD DETECTED! UID: %s\n", uid_str);
        
        uint16_t clue_value = 0;
        
        // Attempt to read the clue block
        if (rfid_read_uint16(RFID_CLUE_BLOCK, &clue_value, &rfid_state)) {
            printf("[RFID] Read clue: %u\n", clue_value);
            
            if(clue_value == 750){
                // Only show the target when the correct card is scanned
                lcd_show_target(750); 
                clue_obtained = true;
                return true;
            }
            else {
                    send_command(LCD_CLEARDISPLAY);
                    lcd_set_cursor(0, 0);
                    lcd_print("Wrong Card!");
                    lcd_set_cursor(0,1);
                    lcd_print("Scan RFID tag");
                    printf("[RFID] Incorrect clue value: %u\n", clue_value);
                    clue_obtained = false;
                    return false;
                }
        } else {
            // Print error if read failed (e.g., Auth failed)
            printf("[RFID] Failed to read clue block %d\n", RFID_CLUE_BLOCK);
            return false;
        }
        
        // Halt the PICC immediately to ensure a clean re-authentication 
        PICC_HaltA(mfrc); 
    }
    
    // If no card or incorrect clue, check if the card is currently present (for puzzle continuation)
    // The original logic only proceeds if rfid_get_clue() returns true *this cycle*.
    // However, since the polling and halting is inside, this function now strictly returns 
    // true only when a NEW card with the correct clue is detected.
    
    // To match the original flow where the check only happens on detection, 
    // we return false if no valid clue was found on polling.
    return false;
}

// Retained for initial setup/testing
bool rfid_write_test_clue() {
    printf("Writing RFID clue %u to block %d...\n", 750, RFID_CLUE_BLOCK);

    if (rfid_write_uint16(RFID_CLUE_BLOCK, 750, &rfid_state)) {
        printf("RFID write OK\n");
        return true;
    } else {
        printf("RFID write FAILED\n");
        return false;
    }
}

void pump_in(){
    water_pump_set_direction(true);
    water_pump_set_speed(pot_percent); 


}
void pump_out(){
    water_pump_set_direction(false);
    water_pump_set_speed(100-pot_percent); 

}
void lcd_update_current(){
    water_percent = read_water_percent();
    lcd_set_cursor(0, 1);
    lcd_print("Water:");
    lcd_print_number(water_percent);
    lcd_print("%   "); // Clear extra characters
}

void lcd_show_success() {
    send_command(LCD_CLEARDISPLAY);
    lcd_set_cursor(0, 0);
    lcd_print("Correct Level!");
    lcd_set_cursor(0, 1);
    lcd_print("Proceed...");
}

void perform_ocr(){
    send_ocr_request();

    if (receive_ocr_response(text_buffer, BUFF_SIZE)) {
        sleep_ms(500);
        LED_off();

        send_command(LCD_CLEARDISPLAY);
        lcd_set_cursor(0, 0);
        lcd_print("Clue:");
        lcd_set_cursor(0, 1);
        lcd_print(text_buffer);

        printf("%s\n", text_buffer);
        } else {
            send_command(LCD_CLEARDISPLAY);
            lcd_print("OCR Timeout!");
            LED_off();
        }
}
int main(){
    stdio_init_all();
    // uart_init_custom();
    
    printf("Station Flow Test\n");
    sleep_ms(2000);

    // ---------- Init Drivers ----------
    lcd_init();
    potentiometer_init();
    button_init();
    water_sensor_init();
    water_pump_init();
    LED_init();
    rfid_init();
    rfid_driver_init(&rfid_state);

    // Optional: write test clue 750 to RFID block (uncomment for first run)
    // rfid_write_test_clue();
    // sleep_ms(2000);

    // Initial Display
    send_command(LCD_CLEARDISPLAY);
    lcd_set_cursor(0, 0);
    lcd_print("Scan RFID Tag");

    // Variables for pot tracking
    int prev_pot = read_potentiometer_mapped(0, 100);

    while(true){
        if(!clue_obtained){
                rfid_get_clue();
        }

        if(clue_obtained && !puzzle_solved){
                printf("[PUZZLE] Clue 750 found. Starting puzzle.\n");
                pot_percent = read_potentiometer_mapped(0, 100);
                printf("[PUZZLE] potentiometer: %d\n", pot_percent);
                int diff = pot_percent - prev_pot;
                printf("[PUZZLE] difference: %d\n", diff);

                // Determine rotation direction
                if (diff > MOVEMENT_THRESHOLD) {
                    printf("[PUZZLE] CW (value increasing)\n");
                    pump_in();
                    prev_pot = pot_percent;
                } else if (diff < -MOVEMENT_THRESHOLD) {
                    printf("[PUZZLE] CCW (value decreasing)\n");
                    pump_out();
                    prev_pot = pot_percent;
                }
                lcd_update_current();
                // submit = (was_button_just_pressed())? 1:0;
                if (was_button_just_pressed()){
                    printf("[PUZZLE] Button Pressed! Current water level: %d%%\n", water_percent);
                    float target_level = (float)750 / 10.0f; // 750 -> 75.0%
                    float water_f = (float)water_percent;
                    sleep_ms(250);

                    if (water_f > (target_level + 3) || 
                    water_f < (target_level - 3)) {
                        // Wrong level - show failure message
                        send_command(LCD_CLEARDISPLAY);
                        lcd_set_cursor(0, 0);
                        lcd_print("Wrong Level!");
                        lcd_set_cursor(0, 1);
                        lcd_print("Resetting...");
                        printf("[PUZZLE] Water level incorrect: %d%%. Resetting tank.\n", water_percent);
                        sleep_ms(1000);                
                        // Empty tank to minimum level (pump at full speed)
                        water_pump_set_direction(false);
                        water_pump_set_speed(100); 
                        while (read_water_percent() > 5) { // Pump until near 0%
                            sleep_ms(50);
                        }
                        water_pump_set_speed(0); // Stop pump
                        pumped_out_sucessfully = true;
                        submit = 0;  
                        
                        // Show target again after reset
                        lcd_show_target(750);
                    } else {
                        // Correct level! Show success message
                        printf("[PUZZLE] Correct water level achieved: %d%%\n", water_percent);
                        puzzle_solved = true; // Lock the puzzle state
                        lcd_show_success();
                        sleep_ms(1500);  
                        // Turn on UV LED to reveal hidden text
                        printf("[PUZZLE] Activating UV LED...\n");  
                        LED_on();     
                        sleep_ms(8000);  // Give time for UV to illuminate the hidden text
                        
                        // Scan the revealed text with camera OCR
                        printf("[PUZZLE] Starting OCR scan...\n");
                        perform_ocr();
                        
                        // Keep the clue displayed
                        submit = 0;                               
                    }              
                }    
        }
        sleep_ms(50); // Small polling delay    
    }
}
