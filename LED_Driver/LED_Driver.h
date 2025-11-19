#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include "pico/stdlib.h"     // Primary SDK Standard Library header (includes sleep_ms/us)
#include "hardware/gpio.h"   // SDK GPIO specific functions

// Use the raw GPIO number (17) for SDK functions
#define LED_GPIO_PIN 17 

// Function declarations for our driver
void LED_init();
void LED_on();
void LED_off();
void LED_toggle();

#endif // LED_DRIVER_H
