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
   gpio_put(redPin, true);

   gpio_init(greenPin);
   gpio_set_dir(greenPin, GPIO_OUT);
   gpio_put(greenPin, true);

   gpio_init(bluePin);
   gpio_set_dir(bluePin, GPIO_OUT);
   gpio_put(bluePin, true);
}
