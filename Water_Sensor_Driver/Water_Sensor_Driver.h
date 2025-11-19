#ifndef WATER_SENSOR_H
#define WATER_SENSOR_H

#include "pico/stdlib.h"        // Primary SDK Standard Library header
#include "hardware/adc.h"       // SDK ADC specific functions
#include "hardware/gpio.h"      // SDK GPIO functions
#include <stdint.h>
#include "utils.h"

// Use the raw GPIO number (28) for SDK functions
#define WATER_PIN_GPIO 28 
// The ADC input channel for GPIO 28 is ADC channel 2
#define WATER_ADC_CHANNEL 2

// Define min/max raw ADC values for calibration
#define WATER_MIN_VALUE 320  // Value when water level is lowest/dry
#define WATER_MAX_VALUE 640 // Value when water level is highest/wet
//max 640 //mid 615 //low 320

// Define calibration points for nonlinear mapping:
#define CALIB_DRY_RAW   22 // New minimum value (0% level)
#define CALIB_MID_RAW   2200 // New midpoint value (50% level)
#define CALIB_WET_RAW   2330 // New maximum value (100% level)

// Function declarations
void water_sensor_init();
int read_water_raw();
int read_water_percent();


#endif