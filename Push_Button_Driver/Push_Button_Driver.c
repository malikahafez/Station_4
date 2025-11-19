#include "Push_Button_Driver.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"

// Define standard logic levels used in C/Pico SDK
#define HIGH true
#define LOW false

// Assuming DEBOUNCE_DELAY is defined in the header (e.g., 50ms)

static volatile bool button_state = HIGH;      // Default state is HIGH (not pressed with pull-up)
static volatile bool last_button_state = HIGH; // Previous reading is HIGH
// Time variables need to store milliseconds using absolute_time_us_t 
static absolute_time_t last_debounce_time_abs; // Use Pico's absolute time structure

// Helper function to get current time in milliseconds (Pico SDK style)
long get_millis() {
    // absolute_time_us_t is based on microseconds, divide by 1000 for milliseconds
    return to_ms_since_boot(get_absolute_time());
}

void button_init() {
    // Use SDK functions instead of pinMode(BUTTON_PIN, INPUT_PULLUP);
    gpio_init(BUTTON_PIN);           // Initialize the pin
    gpio_set_dir(BUTTON_PIN, GPIO_IN); // Set direction to input
    gpio_pull_up(BUTTON_PIN);        // Enable internal pull-up resistor

    // Initialize the debounce timer start point
    last_debounce_time_abs = get_absolute_time();
}

bool is_button_pressed() {
    // Use the SDK function gpio_get() instead of digitalRead()
    // gpio_get() returns true (HIGH) or false (LOW)
    bool reading = gpio_get(BUTTON_PIN); 

    if (reading != last_button_state) {
        // Record the time of the first change
        last_debounce_time_abs = get_absolute_time();
    }

    // Calculate time elapsed since last state change in milliseconds
    long time_since_change = to_ms_since_boot(get_absolute_time()) - to_ms_since_boot(last_debounce_time_abs);

    if (time_since_change > DEBOUNCE_DELAY) {
        if (reading != button_state) {
            button_state = reading;
        }
    }

    last_button_state = reading;
    
    // Logic Inversion: Return TRUE only when the physical pin reads LOW (pressed)
    return (button_state == LOW); 
}

bool was_button_just_pressed() {
    static bool previous_press_state = false;
    // Call the updated is_button_pressed which uses Pico SDK time functions
    bool current_press_state = is_button_pressed(); 

    // Check for a transition from not-pressed (false) to pressed (true)
    if (current_press_state == true && previous_press_state == false) {
        previous_press_state = current_press_state;
        return true; // A new press event occurred
    }
    
    previous_press_state = current_press_state;
    return false; // No new press event
}
