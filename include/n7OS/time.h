#ifndef __TIME_H__
#define __TIME_H__

#include <inttypes.h>

#define OSC_FREQ 0x1234BD
#define TIMER_FREQ 1000

#define OSC_DATA_PORT 0x40
#define OSC_COMMAND_PORT 0x43

extern uint32_t timer_ticks;

typedef struct {
    uint32_t sec;
    uint32_t min;
    uint32_t hours;
} time_t;

/* Initialiser le timer */
void init_timer();

/* Obtenir le temps à partir du nombre de ticks */
time_t get_time_from_ticks(uint32_t ticks);

#endif