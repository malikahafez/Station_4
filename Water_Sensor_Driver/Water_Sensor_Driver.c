#include "Water_Sensor_Driver.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <stdio.h> // Standard C library



void water_sensor_init() {
    // Initialize the ADC hardware block
    adc_init();

    // Initialize GPIO 28 for use as an ADC input pin
    adc_gpio_init(WATER_PIN_GPIO);
    
    // Set ADC resolution if needed (12-bit by default in SDK, 0-4095 range)
    // adc_set_temp_sensor_enabled(false); // Ensure temp sensor is off if not needed
}



int read_water_raw() {
    // Select the correct ADC channel (Channel 2 corresponds to GPIO 28/A2)
    adc_select_input(WATER_ADC_CHANNEL);
    
    // Read the value from the ADC
    return adc_read();
}

int read_water_percent() {
    int raw_value = read_water_raw();
    int percentage = 0;

    // Constrain the incoming value to our total observed range using the C helper
    int constrained_raw = constrain_value(raw_value, CALIB_DRY_RAW, CALIB_WET_RAW);

    // Check if the reading is in the bottom half of the range (0% to 50%)
    if (constrained_raw <= CALIB_MID_RAW) {
        // Map the range [Dry...Mid] to the percentage [0...50] using the C helper
        percentage = map_value(constrained_raw, CALIB_DRY_RAW, CALIB_MID_RAW, 0, 50);
    } 
    // Check if the reading is in the top half of the range (50% to 100%)
    else {
        // Map the range [Mid...Wet] to the percentage [50...100] using the C helper
        percentage = map_value(constrained_raw, CALIB_MID_RAW, CALIB_WET_RAW, 50, 100);
    }

    // Ensure the final output is tightly constrained between 0 and 100
    return constrain_value(percentage, 0, 100);
}