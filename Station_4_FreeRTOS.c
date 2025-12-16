#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "LCD_Driver.h"
#include "Potentiometer_Driver.h"
#include "Push_Button_Driver.h"
#include "Water_Sensor_Driver.h"
#include "Water_Pump_Driver.h"
#include "LED_Driver.h"
#include "RFID_Reader_Driver.h"
#include "utils.h"

// Movement threshold for potentiometer
#define MOVEMENT_THRESHOLD 1

// Queue handles for inter-task communication
QueueHandle_t potQueue;
QueueHandle_t waterQueue;
QueueHandle_t buttonQueue;

// Mutex for LCD access
SemaphoreHandle_t lcdMutex;

// Shared state
typedef struct {
    int pot_percent;
    int water_percent;
    bool pump_on;
} SystemState_t;

SystemState_t systemState = {0, 0, false};

// ===== TASK 1: Read Potentiometer =====
void vPotentiometerTask(void *pvParameters) {
    uint16_t prev = 0;
    
    for (;;) {
        int pot_percent = read_potentiometer_mapped(0, 100);
        int diff = pot_percent - prev;

        if (abs(diff) > MOVEMENT_THRESHOLD) {
            // Send updated value to queue
            xQueueOverwrite(potQueue, &pot_percent);
            
            if (diff > 0) {
                printf("CW (value increasing)\n");
            } else {
                printf("CCW (value decreasing)\n");
            }
            prev = pot_percent;
        }
        
        vTaskDelay(pdMS_TO_TICKS(50)); // 50ms poll rate
    }
}

// ===== TASK 2: Read Water Sensor =====
void vWaterSensorTask(void *pvParameters) {
    for (;;) {
        int water_percent = read_water_percent();
        xQueueOverwrite(waterQueue, &water_percent);
        
        vTaskDelay(pdMS_TO_TICKS(100)); // 100ms poll rate
    }
}

// ===== TASK 3: Button Handler =====
void vButtonTask(void *pvParameters) {
    for (;;) {
        if (was_button_just_pressed()) {
            bool toggle = true;
            xQueueSend(buttonQueue, &toggle, 0);
            printf("Button pressed!\n");
        }
        
        vTaskDelay(pdMS_TO_TICKS(50)); // 50ms debounce
    }
}

// ===== TASK 4: Pump Control =====
void vPumpControlTask(void *pvParameters) {
    int pot_percent = 0;
    bool button_pressed;
    
    for (;;) {
        // Check for button press
        if (xQueueReceive(buttonQueue, &button_pressed, 0) == pdTRUE) {
            systemState.pump_on = !systemState.pump_on;
            
            if (systemState.pump_on) {
                water_pump_set_direction(true);
                printf("Pump ON.\n");
                LED_toggle();
            } else {
                water_pump_off();
                printf("Pump OFF.\n");
                LED_toggle();
            }
        }
        
        // Update pump speed based on potentiometer
        if (xQueuePeek(potQueue, &pot_percent, 0) == pdTRUE) {
            systemState.pot_percent = pot_percent;
            
            if (systemState.pump_on) {
                water_pump_set_speed(pot_percent);
            } else {
                water_pump_set_speed(0);
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// ===== TASK 5: LCD Display Update =====
void vLCDTask(void *pvParameters) {
    int pot_percent = 0;
    int water_percent = 0;
    
    for (;;) {
        // Get latest sensor values
        xQueuePeek(potQueue, &pot_percent, 0);
        xQueuePeek(waterQueue, &water_percent, 0);
        
        // Update LCD with mutex protection
        if (xSemaphoreTake(lcdMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            lcd_set_cursor(0, 1);
            lcd_print("Pot:");
            lcd_print_number(pot_percent);
            lcd_print("% W:");
            lcd_print_number(water_percent);
            lcd_print("%  ");
            
            xSemaphoreGive(lcdMutex);
        }
        
        printf("Pot: %d%% | Water: %d%% | Pump: %s\n", 
               pot_percent, water_percent, systemState.pump_on ? "ON" : "OFF");
        
        vTaskDelay(pdMS_TO_TICKS(200)); // 200ms update rate
    }
}

// ===== TASK 6: RFID Reader =====
void vRFIDTask(void *pvParameters) {
    bool waiting_for_removal = false;
    
    for (;;) {
        if (rfid_is_card_present()) {
            if (!waiting_for_removal) {
                const char* uid = rfid_read_card_uid();
                
                if (uid) {
                    printf("Card detected! UID = %s\n", uid);
                    
                    // Update LCD with mutex
                    if (xSemaphoreTake(lcdMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                        lcd_set_cursor(0, 0);
                        lcd_print(uid);
                        xSemaphoreGive(lcdMutex);
                    }
                } else {
                    printf("Card detected but UID read failed!\n");
                }
                
                waiting_for_removal = true;
            }
        } else {
            if (waiting_for_removal) {
                printf("Card removed.\n");
                
                // Restore default LCD display
                if (xSemaphoreTake(lcdMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                    lcd_set_cursor(0, 0);
                    lcd_print("System Test     ");
                    xSemaphoreGive(lcdMutex);
                }
            }
            waiting_for_removal = false;
        }
        
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

// ===== MAIN =====
int main() {
    // Initialize standard I/O
    stdio_init_all();
    sleep_ms(1000);
    printf("Pico FreeRTOS Multi-Driver System Started.\n");
    
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
    
    // Create queues
    potQueue = xQueueCreate(1, sizeof(int));
    waterQueue = xQueueCreate(1, sizeof(int));
    buttonQueue = xQueueCreate(5, sizeof(bool));
    
    // Create mutex for LCD
    lcdMutex = xSemaphoreCreateMutex();
    
    // Create tasks with priorities
    xTaskCreate(vPotentiometerTask, "PotTask", 256, NULL, 2, NULL);
    xTaskCreate(vWaterSensorTask, "WaterTask", 256, NULL, 2, NULL);
    xTaskCreate(vButtonTask, "ButtonTask", 256, NULL, 3, NULL);
    xTaskCreate(vPumpControlTask, "PumpTask", 256, NULL, 3, NULL);
    xTaskCreate(vLCDTask, "LCDTask", 512, NULL, 1, NULL);
    xTaskCreate(vRFIDTask, "RFIDTask", 512, NULL, 2, NULL);
    
    printf("Tasks created. Starting scheduler...\n");
    
    // Start FreeRTOS scheduler
    vTaskStartScheduler();
    
    // Should never reach here
    while (1) {
        tight_loop_contents();
    }
    
    return 0;
}
