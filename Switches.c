#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "util.h"

#include "Switches.h"

enum
{
   PollRateMs = 60,
};

struct Switches
{
   Switch_Callback_t callback;
   void *context;

   bool switchStates[Switches_Max];
   repeating_timer_t timer;
};
static struct Switches instance;

#define SWITCHES_EXPAND_AS_PINS(Name, Pin, Active, Pull) Pin,

static const uint8_t SwitchPins[Switches_Max] =
{
   SWITCH_TABLE(SWITCHES_EXPAND_AS_PINS)
};

#define SWITCHES_EXPAND_AS_ACTIVE_STATE(Name, Pin, Active, Pull) Active,

static const bool SwitchActiveStates[Switches_Max] =
{
   SWITCH_TABLE(SWITCHES_EXPAND_AS_ACTIVE_STATE)
};

#define SWITCHES_EXPAND_AS_PULL(Name, Pin, Active, Pull) Pull,

static const uint8_t SwitchPulls[Switches_Max] =
{
   SWITCH_TABLE(SWITCHES_EXPAND_AS_PULL)
};

static void InitHardware()
{
   for(uint8_t id; id < Switches_Max; id++)
   {
      uint8_t pin = SwitchPins[id];
      gpio_init(pin);
      gpio_set_dir(pin, GPIO_IN);

      uint8_t pull = SwitchPulls[id];
      bool pullUp = (pull == Switch_Pull_Up) || (pull == Switch_Pull_Both);
      bool pullDown = (pull == Switch_Pull_Down) || (pull == Switch_Pull_Both);

      gpio_set_pulls(pin, pullUp, pullDown);
   }
}

static void ClearInitialStates()
{
   for(uint8_t id; id < Switches_Max; id++)
   {
      bool inactive = !SwitchActiveStates[id];
      instance.switchStates[id] = inactive;
   }
}

static bool poll(repeating_timer_t *rt)
{
   IGNORE(rt);

   for(uint8_t id; id < Switches_Max; id++)
   {
      bool active = SwitchActiveStates[id];
      bool inactive = !active;
      bool previous = instance.switchStates[id];
      bool current = gpio_get(SwitchPins[id]);

      if(current == active && previous == inactive)
      {
         instance.callback(instance.context, Switch_Event_Press, id);
      }
      else if(current == inactive && previous == active)
      {
         instance.callback(instance.context, Switch_Event_Release, id);
      }

      instance.switchStates[id] = current;
   }

   return true;
}

void Switches_Init(Switch_Callback_t callback, void *context)
{
   instance.callback = callback;
   instance.context = context;

   InitHardware();
   ClearInitialStates();

   add_repeating_timer_ms(PollRateMs, poll, NULL, &instance.timer);
}