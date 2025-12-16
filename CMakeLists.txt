# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(Station_4 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# Add FreeRTOS support
set(FREERTOS_KERNEL_PATH ${PICO_SDK_PATH}/lib/tinyusb/lib/FreeRTOS-Kernel)

add_executable(Station_4_FreeRTOS
    Station_4_FreeRTOS.c
    # Add your driver files here
    LCD_Driver.c
    Potentiometer_Driver.c
    Push_Button_Driver.c
    Water_Sensor_Driver.c
    Water_Pump_Driver.c
    LED_Driver.c
    RFID_Reader_Driver.c
    utils.c
)

target_link_libraries(Station_4_FreeRTOS
    pico_stdlib
    hardware_i2c
    hardware_adc
    hardware_pwm
    hardware_spi
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

pico_enable_stdio_usb(Station_4_FreeRTOS 1)
pico_enable_stdio_uart(Station_4_FreeRTOS 0)

pico_add_extra_outputs(Station_4_FreeRTOS)

