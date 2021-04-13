#include "hardware/gpio.h"
#include "util.h"

#include "Switches.h"

#define SWITCHES_EXPAND_AS_PINS(Name, Pin, Active) Pin,

static const bool SwitchPins[Switches_Max] =
{
   SWITCH_TABLE(SWITCHES_EXPAND_AS_PINS)
};

#define SWITCHES_EXPAND_AS_ACTIVE_STATE(Name, Pin, Active) Active,

static const bool SwitchActiveStates[Switches_Max] =
{
   SWITCH_TABLE(SWITCHES_EXPAND_AS_ACTIVE_STATE)
};

static void InitHardware()
{
   for(uint8_t i; i < Switches_Max; i++)
   {
      uint8_t pin = SwitchPins[i];
      gpio_init(pin);
      gpio_set_dir(pin, GPIO_IN);
      // Todo: add pull up/down support
      // gpio_pull_up(pin);
   }
}

void Switches_Init(Switch_Callback_t callback, void *context)
{
   InitHardware();
}