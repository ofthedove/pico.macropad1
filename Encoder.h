#ifndef ENCODER_H
#define ENCODER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "hardware/timer.h"

typedef struct {
   bool dt :1;
   bool clk :1;
   bool sw :1;
} Encoder_InputState_t;

typedef struct {
   uint dtPin;
   uint clkPin;
   uint swPin;
   repeating_timer_t timer;
   queue_t pollQueue;
   Encoder_InputState_t previousState;
} Encoder_t;

void Encoder_Init(Encoder_t *instance, uint dt, uint clk, uint sw);

void Encoder_Run(Encoder_t *instance);

#endif