#include "Potentiometer_Driver.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <stdio.h> // Included for general C stdio functions if needed elsewhere



void potentiometer_init() {
    // Initialize the ADC hardware
    adc_init();

    // Initialize GPIO 29 for use as an ADC input pin
    adc_gpio_init(POTENTIOMETER_PIN_GPIO);

    // The resolution is 12-bit by default (0-4095) in the SDK.
}

int read_potentiometer_value() {
    // Select the correct ADC channel (Channel 3 corresponds to GPIO 29/A3)
    adc_select_input(POT_ADC_CHANNEL);
    
    // Read the value from the ADC
    return adc_read();
}

// Returns 0-100% with rounding
int read_potentiometer_mapped(int low_map, int high_map) {
    // Read raw ADC value (0–4095)
    int raw_value = read_potentiometer_value();

    // Map it directly to the desired range
    int mapped_value = map_value_rounded(raw_value, 0, 4095, low_map, high_map);

    // Constrain output just in case
    return constrain_value(mapped_value, low_map, high_map);
}



