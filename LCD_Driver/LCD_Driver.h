#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <stdint.h>          // Include this for standard integer types like uint8_t
#include "pico/stdlib.h"     // Primary SDK Standard Library header
#include "hardware/i2c.h"    // SDK I2C specific functions

// --- HARDWARE CONFIGURATION ---
// Define which I2C peripheral to use and which GPIO pins it connects to.
#define I2C_PORT i2c0       // Use i2c0 peripheral
#define SDA_PIN  12         // SDA connected to GPIO 12
#define SCL_PIN  13         // SCL connected to GPIO 13
// ------------------------------

// Define the I2C address (common default is 0x27)
#define LCD_ADDRESS 0x27

// Define the pin mapping on the PCF8574 I/O expander
#define RS_PIN      0
#define RW_PIN      1
#define EN_PIN      2
#define BL_PIN      3   // Backlight pin (P3)
#define D4_PIN      4
#define D5_PIN      5
#define D6_PIN      6
#define D7_PIN      7

// Commands for the HD44780 LCD controller
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// Flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Flags for display control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// Flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// --- Function Prototypes ---
// The original line: void lcd_print_float(double number, unsigned int decimals = 2);
// Is modified to this standard C format:
void lcd_print_float(double number, unsigned int decimals); 

// All other prototypes remain the same:
void i2c_send_byte(uint8_t data);
void send_nibble(uint8_t nibble, uint8_t mode);
void send_command(uint8_t command);
void send_data(uint8_t data);
void lcd_init();
void lcd_print(const char* str);
void lcd_print_number(unsigned long number);
void lcd_set_cursor(uint8_t col, uint8_t row);

#endif
