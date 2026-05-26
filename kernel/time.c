#include <n7OS/time.h>
#include <n7OS/cpu.h>

uint32_t timer_ticks = 0;

void init_timer() {

    outb(0x34, OSC_COMMAND_PORT);

    uint32_t divisor = OSC_FREQ / TIMER_FREQ;

    outb(divisor & 0xFF, OSC_DATA_PORT);
    outb((divisor >> 8) & 0xFF, OSC_DATA_PORT);
}

time_t get_time_from_ticks(uint32_t ticks) {
    time_t t;
    uint32_t seconds = ticks / TIMER_FREQ;
    t.sec = seconds%60;
    t.min = (seconds / 60) % 60;
    t.hours = (seconds / 3600) % 24;
    return t;
}