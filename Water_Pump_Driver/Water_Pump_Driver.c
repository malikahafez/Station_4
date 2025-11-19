#include "Water_Pump_Driver.h"

// We need to keep track of the PWM slice and channel assigned to the enable pin
static uint slice_num;
static uint channel_num;

void water_pump_init() {
    // 1. Initialize Direction Pins as GPIO outputs
    gpio_init(DIR1_GPIO_PIN);
    gpio_set_dir(DIR1_GPIO_PIN, GPIO_OUT);
    gpio_init(DIR2_GPIO_PIN);
    gpio_set_dir(DIR2_GPIO_PIN, GPIO_OUT);

    // Start with the pump off and direction neutral
    gpio_put(DIR1_GPIO_PIN, 0);
    gpio_put(DIR2_GPIO_PIN, 0);

    // 2. Initialize Enable Pin (Speed Control) using PWM
    gpio_set_function(En1_PWM_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(En1_PWM_PIN);
    channel_num = pwm_gpio_to_channel(En1_PWM_PIN);

    // Set PWM wrap value (e.g., 255 for 8-bit resolution, common for H-Bridges)
    pwm_set_wrap(slice_num, 255);
    pwm_set_chan_level(slice_num, channel_num, 0); // Start at 0 speed
    pwm_set_enabled(slice_num, true); // Enable PWM
}

void water_pump_set_direction(bool forward) {
    if (forward) {
        // Forward: DIR1 HIGH, DIR2 LOW
        gpio_put(DIR1_GPIO_PIN, 1);
        gpio_put(DIR2_GPIO_PIN, 0);
    } else {
        // Reverse: DIR1 LOW, DIR2 HIGH
        gpio_put(DIR1_GPIO_PIN, 0);
        gpio_put(DIR2_GPIO_PIN, 1);
    }
}

void water_pump_set_speed(int speed_percent) {
    // Constrain the input to a valid range
    if (speed_percent < 0) speed_percent = 0;
    if (speed_percent > 100) speed_percent = 100;

    // Map the 0-100% range to the 0-255 PWM wrap value
    uint16_t pwm_level = (uint16_t)((speed_percent / 100.0) * 255);

    pwm_set_chan_level(slice_num, channel_num, pwm_level);
}

void water_pump_off() {
    water_pump_set_speed(0); // Set speed to zero
    // Optional: Stop all direction pins for safe shutdown
    gpio_put(DIR1_GPIO_PIN, 0);
    gpio_put(DIR2_GPIO_PIN, 0);
}