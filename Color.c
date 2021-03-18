#include "util.h"
#include "Color.h"

HsvColor_t ColorRgbToHsv(RgbColor_t rgbIn)
{
   HsvColor_t hsvOut;

   double min = MIN(MIN(rgbIn.red, rgbIn.green), rgbIn.blue);
   double max = MAX(MAX(rgbIn.red, rgbIn.green), rgbIn.blue);
   double delta = max - min;

   hsvOut.value = max;

   if(delta < 0.00001)
   {
      hsvOut.saturation = 0;
      hsvOut.hue = 0; // undefined
      return hsvOut;
   }

   if(max > 0.0)
   {
      // NOTE: if Max is == 0, this divide would cause a crash
      hsvOut.saturation = (delta / max);
   }
   else
   {
      // if max is 0, then r = g = b = 0 => s = 0, h is undefined
      hsvOut.saturation = 0.0;
      hsvOut.hue = 0; // undefined
      return hsvOut;
   }

   double hue;
   if(rgbIn.red >= max) // > is bogus, just keeps compilor happy
   { // between yellow & magenta
      hue = ( rgbIn.green - rgbIn.blue ) / delta;
   }
   else if(rgbIn.green >= max)
   { // between cyan & yellow
      hue = 2.0 + (rgbIn.blue - rgbIn.red) / delta;
   }
   else
   { // between magenta & cyan
      hue = 4.0 + (rgbIn.red - rgbIn.green) / delta;
   }

   hue *= 60.0; // degrees

   if(hue < 0.0)
   {
      hue += 360.0;
   }

   hsvOut.hue = (uint8_t)hue;

   return hsvOut;
}

RgbColor_t ColorHsvToRgb(HsvColor_t hsvColor)
{

}

/*
rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;     
}
*/
