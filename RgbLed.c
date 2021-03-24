#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "RgbLed.h"

enum
{
   PwmTop = 255,
};

static void ConfigurePwm(uint8_t pin)
{
   gpio_set_function(pin, GPIO_FUNC_PWM);
   uint8_t sliceNum = pwm_gpio_to_slice_num(pin);
   pwm_set_wrap(sliceNum, PwmTop);
   pwm_set_enabled(sliceNum, true);
}

void RgbLed_Init(RgbLed_t *instance, uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
{
   instance->redPin = redPin;
   instance->greenPin = greenPin;
   instance->bluePin = bluePin;

   ConfigurePwm(redPin);
   ConfigurePwm(greenPin);
   ConfigurePwm(bluePin);
}

void RgbLed_Write(RgbLed_t *instance, RgbColor_t color)
{
   pwm_set_gpio_level(instance->redPin, color.red);
   pwm_set_gpio_level(instance->greenPin, color.green);
   pwm_set_gpio_level(instance->bluePin, color.blue);
}
