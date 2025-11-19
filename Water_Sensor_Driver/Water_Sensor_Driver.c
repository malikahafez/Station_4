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

// Map raw water sensor to 0-100% with rounding
int read_water_percent() {
    static int last_raw = 0;        // last stable ADC reading
    const int adc_hysteresis = 5;   // minimum raw ADC change to update

    int raw_value = read_water_raw();

    // Apply hysteresis on the raw ADC value
    if (abs(raw_value - last_raw) < adc_hysteresis) {
        raw_value = last_raw;
    }

    last_raw = raw_value;

    // Constrain the incoming value to observed dry/wet calibration
    int constrained_raw = constrain_value(raw_value, CALIB_DRY_RAW, CALIB_WET_RAW);

    int percentage = 0;

    // Map the constrained value to 0-100% using mid-point splitting
    if (constrained_raw <= CALIB_MID_RAW) {
        percentage = map_value_rounded(constrained_raw, CALIB_DRY_RAW, CALIB_MID_RAW, 0, 50);
    } else {
        percentage = map_value_rounded(constrained_raw, CALIB_MID_RAW, CALIB_WET_RAW, 50, 100);
    }

    return constrain_value(percentage, 0, 100);
}
