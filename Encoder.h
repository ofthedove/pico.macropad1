#ifndef ENCODER_H
#define ENCODER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "hardware/timer.h"

enum
{
   Encoder_Event_RotateCW,
   Encoder_Event_RotateCCW,
   Encoder_Event_ButtonPress,
   Encoder_Event_ButtonRelease,
};
typedef uint8_t Encoder_Event_t;

typedef void (*Encoder_Callback_t)(void *context, Encoder_Event_t event);

typedef struct {
   bool dt :1;
   bool clk :1;
   bool sw :1;
} Encoder_InputState_t;

typedef struct {
   uint dtPin;
   uint clkPin;
   uint swPin;

   Encoder_Callback_t rotateCallback;
   void *rotateContext;
   Encoder_Callback_t buttonCallback;
   void *buttonContext;

   repeating_timer_t timer;
   queue_t pollQueue;
   Encoder_InputState_t previousState;
} Encoder_t;

void Encoder_Init(
   Encoder_t *instance,
   uint dtPin,
   uint clkPin,
   uint swPin,
   Encoder_Callback_t rotateCallback,
   void *rotateContext,
   Encoder_Callback_t buttonCallback,
   void *buttonContext);

void Encoder_Run(Encoder_t *instance);

#endif