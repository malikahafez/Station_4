#ifndef POTENTIOMETER_DRIVER_H
#define POTENTIOMETER_DRIVER_H

#include "pico/stdlib.h"        // Primary SDK Standard Library header
#include "hardware/adc.h"       // SDK ADC specific functions
#include "hardware/gpio.h"      // SDK GPIO functions
#include "utils.h"
#include <stdint.h>

// Use the GPIO number (29) for clarity in the SDK
#define POTENTIOMETER_PIN_GPIO 29 
// The ADC input channel for GPIO 29 is ADC channel 3
#define POT_ADC_CHANNEL 3 

// Function Prototypes
void potentiometer_init();
int read_potentiometer_value();
int read_potentiometer_mapped(int low_map, int high_map);


#endif // POTENTIOMETER_DRIVER_H
