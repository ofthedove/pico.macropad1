#include "hardware/gpio.h"
#include "util.h"

#include "Encoder.h"

enum
{
   PollRateMs = 50,
   BufferSize = 10,
};

static void hardwareInit(Encoder_t *instance) {
   gpio_init(instance->dtPin);
   gpio_set_dir(instance->dtPin, GPIO_IN);

   gpio_init(instance->clkPin);
   gpio_set_dir(instance->clkPin, GPIO_IN);

   gpio_init(instance->swPin);
   gpio_set_dir(instance->swPin, GPIO_IN);
   gpio_pull_up(instance->swPin);
}

static bool poll(repeating_timer_t *rt) {
   REINTERPRET(Encoder_t*, instance, rt->user_data);
   
   Encoder_InputState_t inputStates;
   inputStates.dt = gpio_get(instance->dtPin);
   inputStates.clk = gpio_get(instance->clkPin);
   inputStates.sw = gpio_get(instance->swPin);

   queue_try_add(&instance->pollQueue, &inputStates);

   return ContinueTimer;
}

void Encoder_Init(Encoder_t *instance, uint dt, uint clk, uint sw)
{
   instance->dtPin = dt;
   instance->clkPin = clk;
   instance->swPin = sw;

   hardwareInit(instance);

   if(!add_repeating_timer_ms(PollRateMs, poll, instance, &instance->timer)) {
      // todo: create some kind of logging system with debug enable/disable available from anywhere
      // uart_puts(UART_ID, "Failed to add timer\n");
      while(true);
   }

   queue_init(&instance->pollQueue, sizeof(Encoder_InputState_t), BufferSize);
}

void Encoder_Run(Encoder_t *instance)
{
   while(queue_is_empty(&instance->pollQueue) == false)
   {
      Encoder_InputState_t state;
      queue_try_remove(&instance->pollQueue, &state);

      // Compare to previous state
      // fire events as applicable

      instance->previousState.dt = state.dt;
      instance->previousState.clk = state.clk;
      instance->previousState.sw = state.sw;
   }
}