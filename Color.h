#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef struct
{
   uint8_t red;
   uint8_t green;
   uint8_t blue;
} RgbColor_t;

typedef struct
{
   uint16_t hue;
   uint8_t saturation;
   uint8_t value;
} HsvColor_t;

HsvColor_t Color_RgbToHsv(RgbColor_t rgbColor);

RgbColor_t Color_HsvToRgb(HsvColor_t hsvColor);

#endif