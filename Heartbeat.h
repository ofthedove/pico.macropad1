#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

typedef struct
{
   uint8_t ledPin;
   repeating_timer_t timer;
} Heartbeat_t;

void Heartbeat_Init(Heartbeat_t *instance, uint8_t ledPin);

#endif