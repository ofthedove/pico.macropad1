#ifndef RGBLED_H
#define RGBLED_H

#include <stdint.h>
#include "Color.h"

typedef struct
{
   uint8_t redPin;
   uint8_t greenPin;
   uint8_t bluePin;
} RgbLed_t;

void RgbLed_Init(RgbLed_t *instance, uint8_t redPin, uint8_t greenPin, uint8_t bluePin);

void RgbLed_Write(RgbLed_t *instance, RgbColor_t color);

#endif