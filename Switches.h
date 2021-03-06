#ifndef SWITCHES_H
#define SWITCHES_H

#include <stdint.h>
#include "Color.h"

enum
{
   Switch_Event_Press,
   Switch_Event_Release,
};
typedef uint8_t Switch_Event_t;

enum
{
   Switch_ActiveHigh = 1,
   Switch_ActiveLow = 0,

   Switch_Pull_None,
   Switch_Pull_Up,
   Switch_Pull_Down,
   Switch_Pull_Both,
};

#define SWITCH_TABLE(ENTRY) \
   /*    Name,            GPIO Pin,  Active High/Low,   Pull up/down */ \
   ENTRY(Switch_Button1,  8,         Switch_ActiveLow,  Switch_Pull_Up) \

#define SWITCHES_EXPAND_AS_ENUM(Name, Pin, Active, Pull) Name,

enum
{
   SWITCH_TABLE(SWITCHES_EXPAND_AS_ENUM)
   Switches_Max
};
typedef uint8_t Switches_SwitchName_t;

typedef void (*Switch_Callback_t)(void *context, Switch_Event_t event, Switches_SwitchName_t buttonName);

void Switches_Init(Switch_Callback_t callback, void *context);

#endif