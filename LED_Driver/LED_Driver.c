#include "LED_Driver.h"

void LED_init() {
    // SDK equivalent of pinMode(LED_PIN, OUTPUT);
    gpio_init(LED_GPIO_PIN);           // Initialize the pin
    gpio_set_dir(LED_GPIO_PIN, GPIO_OUT); // Set direction to output
    gpio_put(LED_GPIO_PIN, 0);         // Start with the LED off (equivalent to digitalWrite(LOW))
}

void LED_on() {
    // SDK equivalent of digitalWrite(LED_PIN, HIGH);
    gpio_put(LED_GPIO_PIN, 1);
}

void LED_off() {
    // SDK equivalent of digitalWrite(LED_PIN, LOW);
    gpio_put(LED_GPIO_PIN, 0);
}

void LED_toggle() {
    // Read the current *output* state of the pin and write the inverse state
    bool current_state = gpio_get_out_level(LED_GPIO_PIN);
    gpio_put(LED_GPIO_PIN, !current_state);
}