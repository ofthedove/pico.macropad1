# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/Users/andrewcombs/git/pico/pico-sdk")

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(MacroPad1 C CXX)

# Initialise the Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(
        MacroPad1 MacroPad1.c
        Color Color.c
        Encoder Encoder.c
        Heartbeat Heartbeat.c
        RgbLed RgbLed.c
        Switches Switches.c
        )

pico_set_program_name(MacroPad1 "MacroPad1")
pico_set_program_version(MacroPad1 "0.1")

pico_enable_stdio_uart(MacroPad1 0)
pico_enable_stdio_usb(MacroPad1 1)

# Add the standard library to the build
target_link_libraries(MacroPad1 pico_stdlib)

# Add any user requested libraries
target_link_libraries(MacroPad1
        hardware_i2c
        hardware_pio
        hardware_timer
        hardware_clocks
        hardware_pwm
        )

pico_add_extra_outputs(MacroPad1)

