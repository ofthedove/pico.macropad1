#include "util.h"
#include "Color.h"

// Math based on this: https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both

enum
{
   HueMax = 360,
   RegionSize = 60,
   NumRegions = 6,
};

HsvColor_t Color_RgbToHsv(RgbColor_t rgbIn)
{
   HsvColor_t hsvOut;

   uint8_t min = MIN(MIN(rgbIn.red, rgbIn.green), rgbIn.blue);
   uint8_t max = MAX(MAX(rgbIn.red, rgbIn.green), rgbIn.blue);
   uint8_t delta = max - min;

   hsvOut.value = max;

   if(max == 0 || delta == 0)
   {
      hsvOut.saturation = 0;
      hsvOut.hue = 0; // undefined
      return hsvOut;
   }

   uint16_t temp = (uint16_t)255 * (uint16_t)delta;
   temp = temp / (uint16_t)max;
   hsvOut.saturation = (uint8_t)temp;

   if(rgbIn.red == max)
   { // between yellow & magenta
      int16_t temp;
      temp = (int16_t)rgbIn.green - (int16_t)rgbIn.blue;
      temp = temp / (int16_t)delta;
      temp = temp * RegionSize;
      hsvOut.hue = (0 * RegionSize) + temp;
   }
   else if(rgbIn.green == max)
   { // between cyan & yellow
      int16_t temp;
      temp = (int16_t)rgbIn.blue - (int16_t)rgbIn.red;
      temp = temp / (int16_t)delta;
      temp = temp * RegionSize;
      hsvOut.hue = (2 * RegionSize) + temp;
   }
   else
   { // between magenta & cyan
      int16_t temp;
      temp = (int16_t)rgbIn.red - (int16_t)rgbIn.green;
      temp = temp / (int16_t)delta;
      temp = temp * RegionSize;
      hsvOut.hue = (4 * RegionSize) + temp;
   }

   return hsvOut;
}

RgbColor_t Color_HsvToRgb(HsvColor_t hsvIn)
{
   RgbColor_t rgbOut;

   if(hsvIn.saturation == 0)
   {
      rgbOut.red = hsvIn.value;
      rgbOut.green = hsvIn.value;
      rgbOut.blue = hsvIn.value;
      return rgbOut;
   }

   uint16_t hue = hsvIn.hue;
   hue %= HueMax;
   uint8_t region = hue / RegionSize;
   uint16_t remainder = (hsvIn.hue - (region * RegionSize)) * NumRegions;

   uint16_t value = (uint16_t)hsvIn.value;
   uint16_t saturation = (uint16_t)hsvIn.saturation;

   uint8_t p = (value * (255 - saturation)) >> 8;
   uint8_t q = (value * (255 - ((saturation * remainder) >> 8))) >> 8;
   uint8_t t = (value * (255 - ((saturation * (255 - remainder)) >> 8))) >> 8;

   switch(region)
   {
      case 0:
         rgbOut.red = hsvIn.value;
         rgbOut.green = t;
         rgbOut.blue = p;
         break;
      case 1:
         rgbOut.red = q;
         rgbOut.green = hsvIn.value;
         rgbOut.blue = p;
         break;
      case 2:
         rgbOut.red = p;
         rgbOut.green = hsvIn.value;
         rgbOut.blue = t;
         break;
      case 3:
         rgbOut.red = p;
         rgbOut.green = q;
         rgbOut.blue = hsvIn.value;
         break;
      case 4:
         rgbOut.red = t;
         rgbOut.green = p;
         rgbOut.blue = hsvIn.value;
         break;
      default:
         rgbOut.red = hsvIn.value;
         rgbOut.green = p;
         rgbOut.blue = q;
         break;
   }

   return rgbOut;
}
