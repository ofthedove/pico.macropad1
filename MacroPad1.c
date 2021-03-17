#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart0
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// GPIO defines
// Example uses GPIO 2
#define GPIO 2


// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

enum {
    BuiltinLed = PICO_DEFAULT_LED_PIN,

    On = true,
};

bool timer_callback(repeating_timer_t *rt) {
    // Put your timeout handler code in here

    gpio_put(BuiltinLed, !gpio_get(BuiltinLed));
    // gpio_put(BuiltinLed, false);

    // uart_puts(UART_ID, "timerHit\n");

    return true;
}

// int64_t alarm_callback(alarm_id_t id, void *user_data) {
//     // Put your timeout handler code in here
//     gpio_put(BuiltinLed, !gpio_get(BuiltinLed));
//     return 0;
// }



int main()
{
    stdio_init_all();

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    

    // GPIO initialisation.
    // We will make this GPIO an input, and pull it up by default
    // gpio_init(GPIO);
    // gpio_set_dir(GPIO, GPIO_IN);
    // gpio_pull_up(GPIO);
    
    gpio_init(BuiltinLed);
    gpio_set_dir(BuiltinLed, GPIO_OUT);
    gpio_put(BuiltinLed, On);
    

    // Example of using the HW divider. The pico_divider library provides a more user friendly set of APIs 
    // over the divider (and support for 64 bit divides), and of course by default regular C language integer
    // divisions are redirected thru that library, meaning you can just use C level `/` and `%` operators and
    // gain the benefits of the fast hardware divider.
    // int32_t dividend = 123456;
    // int32_t divisor = -321;
    // // This is the recommended signed fast divider for general use.
    // divmod_result_t result = hw_divider_divmod_s32(dividend, divisor);
    // printf("%d/%d = %d remainder %d\n", dividend, divisor, to_quotient_s32(result), to_remainder_s32(result));
    // // This is the recommended unsigned fast divider for general use.
    // int32_t udividend = 123456;
    // int32_t udivisor = 321;
    // divmod_result_t uresult = hw_divider_divmod_u32(udividend, udivisor);
    // printf("%d/%d = %d remainder %d\n", udividend, udivisor, to_quotient_u32(uresult), to_remainder_u32(uresult));

    // I2C Initialisation. Using it at 400Khz.
    // i2c_init(I2C_PORT, 400*1000);
    
    // gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    // gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    // gpio_pull_up(I2C_SDA);
    // gpio_pull_up(I2C_SCL);


    repeating_timer_t timer;
    if(!add_repeating_timer_ms(500, timer_callback, NULL, &timer)) {
        (void) timer_callback;
        uart_puts(UART_ID, "Failed to add timer\n");
    }
    // add_alarm_in_ms(500, alarm_callback, NULL, false);


    uart_puts(UART_ID, "Hello, world!\n");

    while(true) {
    }

    return 0;
}
