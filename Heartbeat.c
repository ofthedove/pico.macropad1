#include "Heartbeat.h"
#include "util.h"

enum
{
   HalfPeriodMs = 1000,
   LedOn = true,
};

static bool toggleLed(repeating_timer_t *rt)
{
   REINTERPRET(Heartbeat_t*, instance, rt->user_data);

   bool newValue = !gpio_get(instance->ledPin);
   gpio_put(instance->ledPin, newValue);
}

void Heartbeat_Init(Heartbeat_t *instance, uint8_t ledPin)
{
   instance->ledPin = ledPin;

   gpio_init(ledPin);
   gpio_set_dir(ledPin, GPIO_OUT);
   gpio_put(ledPin, LedOn);

   add_repeating_timer_ms(HalfPeriodMs, toggleLed, instance, &instance->timer);
}
