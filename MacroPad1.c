#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#include "util.h"
#include "Color.h"
#include "Encoder.h"
#include "Heartbeat.h"
#include "RgbLed.h"
#include "Switches.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart0
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

enum {
   BuiltinLed = PICO_DEFAULT_LED_PIN,
};

RgbLed_t rgbLed;
bool rgbBlocked = false;

static void encoderRotateCallback(void *context, Encoder_Event_t event)
{
   IGNORE(context);

   // skip every other call b/c encoder counts two counts per click
   static bool toggle;
   toggle = !toggle;
   if(toggle)
   {
      return;
   }

   if(rgbBlocked)
   {
      return;
   }

   static uint16_t hue = 0;

   if(event == Encoder_Event_RotateCW) {
      // uart_puts(UART_ID, "Encoder Rotate CW\r\n");
      hue += 5;
   } else if (event == Encoder_Event_RotateCCW) {
      // uart_puts(UART_ID, "Encoder Rotate CCW\r\n");
      hue -= 5;
   } else {
      uart_puts(UART_ID, "Unknown Rotate Event!\r\n");
   }

   hue %= 360;

   HsvColor_t hsv =
   {
      .hue = hue,
      .saturation = 100,
      .value = 100
   };

   RgbColor_t rgb = Color_HsvToRgb(hsv);

   char myStr[40];
   sprintf(myStr, "Hue %3d  R %3d G %3d B %3d\r\n", hue, rgb.red, rgb.green, rgb.blue);
   uart_puts(UART_ID, myStr);

   RgbLed_Write(&rgbLed, rgb);
}

static void switchesCallback(void *context, Switch_Event_t event, Switches_SwitchName_t buttonName)
{
   IGNORE(context);

   switch(buttonName)
   {
      case Switch_Button1:
         if(event == Switch_Event_Press)
         {
            RgbColor_t rgb = {0, 0, 0};
            RgbLed_Write(&rgbLed, rgb);

            rgbBlocked = true;
         }
         else if(event == Switch_Event_Release)
         {
            rgbBlocked = false;
         }
         break;
   }
}

static void encoderButtonCallback(void *context, Encoder_Event_t event)
{
   IGNORE(context);

   if(event == Encoder_Event_ButtonPress) {
      uart_puts(UART_ID, "Encoder Button Press\r\n");
   } else if (event == Encoder_Event_ButtonRelease) {
      uart_puts(UART_ID, "Encoder Button Release\r\n");
   } else {
      uart_puts(UART_ID, "Unknown Button Event!\r\n");
   }
}

int main()
{
   stdio_init_all();

   // Set up our UART
   uart_init(UART_ID, BAUD_RATE);
   // Set the TX and RX pins by using the function select on the GPIO
   // Set datasheet for more information on function select
   gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
   gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

   // I2C Initialisation. Using it at 400Khz.
   // i2c_init(I2C_PORT, 400*1000);

   // gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
   // gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
   // gpio_pull_up(I2C_SDA);
   // gpio_pull_up(I2C_SCL);

   uart_puts(UART_ID, "Hello, world!\r\n");


   Heartbeat_t heartbeat;
   Heartbeat_Init(&heartbeat, BuiltinLed);

   RgbLed_Init(&rgbLed, 5, 6, 7);

   Encoder_t encoder;
   Encoder_Init(
      &encoder,
      3,
      4,
      2,
      encoderRotateCallback,
      NULL,
      encoderButtonCallback,
      NULL);

   Switches_Init(switchesCallback, NULL);

   while(true) {
      Encoder_Run(&encoder);
   }

   return 0;
}
