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

// Returns 0-100% with rounding, stable at edges
int read_potentiometer_mapped(int low_map, int high_map) {
    static int last_raw = 0;           // last stable ADC reading
    const int adc_hysteresis = 5;      // minimum raw ADC change to update

    int raw_value = read_potentiometer_value();

    // Apply hysteresis on the raw ADC value
    if (abs(raw_value - last_raw) < adc_hysteresis) {
        raw_value = last_raw;
    }

    last_raw = raw_value;

    // Map raw ADC value to 0-100% (or low_map-high_map)
    int mapped_value = map_value_rounded(raw_value, 0, 4095, low_map, high_map);

    // Constrain final output just in case
    return constrain_value(mapped_value, low_map, high_map);
}


