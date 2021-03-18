#include "hardware/gpio.h"
#include "util.h"

#include "Encoder.h"

enum
{
   PollRateMs = 5,
   BufferSize = 10,

   Pressed = false,
};

const short int EncoderArray[4][4] =
  {{ 0, 1,-1, 0 },
   {-1, 0, 0, 1 },
   { 1, 0, 0,-1 },
   { 0,-1, 1, 0 }};

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

void Encoder_Init(
   Encoder_t *instance,
   uint dtPin,
   uint clkPin,
   uint swPin,
   Encoder_Callback_t rotateCallback,
   void *rotateContext,
   Encoder_Callback_t buttonCallback,
   void *buttonContext)
{
   instance->dtPin = dtPin;
   instance->clkPin = clkPin;
   instance->swPin = swPin;

   instance->rotateCallback = rotateCallback;
   instance->rotateContext = rotateContext;
   instance->buttonCallback = buttonCallback;
   instance->buttonContext = buttonContext;

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

      if(instance->previousState.dt != state.dt || instance->previousState.clk != state.clk)
      {
         uint8_t previousEncoder = (instance->previousState.dt << 1) + instance->previousState.clk;
         uint8_t currentEncoder = (state.dt << 1) + state.clk;

         uint8_t encoderChange = EncoderArray[previousEncoder][currentEncoder];

         if(encoderChange != 0)
         {
            Encoder_Event_t event = encoderChange == 1 ? Encoder_Event_RotateCW : Encoder_Event_RotateCCW;

            if(instance->rotateCallback != NULL)
            {
               instance->rotateCallback(instance->rotateContext, event);
            }
         }
      }

      if(instance->previousState.sw != state.sw)
      {
         Encoder_Event_t event = (state.sw == Pressed ? Encoder_Event_ButtonPress : Encoder_Event_ButtonRelease);

         if(instance->buttonCallback != NULL)
         {
            instance->buttonCallback(instance->buttonContext, event);
         }
      }

      instance->previousState.dt = state.dt;
      instance->previousState.clk = state.clk;
      instance->previousState.sw = state.sw;
   }
}