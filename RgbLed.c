#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "RgbLed.h"

void RgbLed_Init(RgbLed_t *instance, uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
{
   instance->redPin = redPin;
   instance->greenPin = greenPin;
   instance->bluePin = bluePin;

   gpio_init(redPin);
   gpio_set_dir(redPin, GPIO_OUT);
   gpio_put(redPin, false);

   gpio_init(greenPin);
   gpio_set_dir(greenPin, GPIO_OUT);
   gpio_put(greenPin, false);

   gpio_init(bluePin);
   gpio_set_dir(bluePin, GPIO_OUT);
   gpio_put(bluePin, false);
}

void RgbLed_Write(RgbLed_t *instance, RgbColor_t color)
{
   gpio_put(instance->redPin, (color.red >= 128));
   gpio_put(instance->greenPin, (color.green >= 128));
   gpio_put(instance->bluePin, (color.blue >= 128));
}
