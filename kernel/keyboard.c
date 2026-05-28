#include <n7OS/keyboard.h>

/* define keyboard state variables here (one translation unit) */
uint8_t is_shift_pressed = 0;
uint8_t is_ctrl_pressed = 0;
uint8_t is_alt_pressed = 0;

uint16_t key_buffer[256];
uint8_t buffer_size = 0;
uint8_t buffer_start = 0;

void init_keyboard() {
    is_alt_pressed = 0;
    is_ctrl_pressed = 0;
    is_shift_pressed = 0;

    buffer_start = 0;
    buffer_size = 0;
}

uint16_t kgetch() {
    if (buffer_size == 0) {
        return 0;
    }

    uint16_t c = key_buffer[buffer_start];
    buffer_start = (buffer_start + 1) % 256;
    buffer_size--;

    return c;
}