#ifndef PUSH_BUTTON_DRIVER_H
#define PUSH_BUTTON_DRIVER_H

#include <stdbool.h> // For bool type
#include <stdint.h>  // For uint types

// Configuration
#define BUTTON_PIN      16  // Example GPIO Pin 15
#define DEBOUNCE_DELAY  50  // Debounce time in milliseconds

// Function Prototypes
void button_init();
bool is_button_pressed();
bool was_button_just_pressed();

#endif // PUSH_BUTTON_DRIVER_H