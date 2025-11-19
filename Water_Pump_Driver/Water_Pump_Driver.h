#ifndef WATER_PUMP_DRIVER_H
#define WATER_PUMP_DRIVER_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <stdbool.h>
#include <stdint.h>

#define En1_PWM_PIN 18 //enable pin, controls speed D6/GPIO18
#define DIR1_GPIO_PIN 15 //direction pin 1 D3/GPIO15
#define DIR2_GPIO_PIN 25 //direction pin 2 D2/GPIO25

// Function declarations
void water_pump_init();
void water_pump_set_speed(int speed_percent); // Speed from 0 (off) to 100 (%)
void water_pump_set_direction(bool forward);  // true for forward, false for reverse
void water_pump_off();

#endif//WATER_PUMP_DRIVER_H