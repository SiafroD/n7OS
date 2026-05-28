#include <inttypes.h>
#include <n7OS/irq.h>
#include <n7OS/time.h>
#include <n7OS/cpu.h>
#include <n7OS/keyboard.h>

extern void handler_IT_timer();
extern void handler_IT_keyboard();

void handler_timer() {
    outb(0x20, PIC_COMMAND_PORT); 
    timer_ticks++;
}

void handler_keyboard() {
    outb(0x20, PIC_COMMAND_PORT); 
    uint8_t scancode = inb(KEYB_ENCODER);

    switch(scancode) {
        case SHIFT_PRESSED:
            is_shift_pressed = 1;
            break;
        case SHIFT_RELEASED:
            is_shift_pressed = 0;
            break;
        case CTRL_PRESSED:
            is_ctrl_pressed = 1;
            break;
        case CTRL_RELEASED:
            is_ctrl_pressed = 0;
            break;
        case ALT_PRESSED:
            is_alt_pressed = 1;
            break;
        case ALT_RELEASED:
            is_alt_pressed = 0;
            break;
        default:
            if (!IS_KEY_RELEASED(scancode)) {
                char key;
                if (is_shift_pressed) {
                    key = scancode_map_shift[scancode];
                } else {
                    key = scancode_map[scancode];
                }

                if (key != 0) {
                    key_buffer[(buffer_size + buffer_start) % 256] = key;
                    buffer_size++;
                }
            }
            

            break;
    }
}


void init_irq_entry(int irq_num, uint32_t addr) {
    idt_entry_t *entry = (idt_entry_t *)&idt[irq_num];
    entry->offset_inf = addr & 0xFFFF;
    entry->zero = 0x00;
    entry->offset_sup = addr >> 16;
    entry->sel_segment = KERNEL_CS; 
    entry->type_attr = 0x8E;

}

void init_irq() {
    init_irq_keyboard();
    init_irq_timer();
}

void init_irq_timer() {
    init_irq_entry(TIMER_INTERRUPT_ID, (uint32_t)handler_IT_timer);

    outb(inb(PIC_DATA_PORT)&~(1 << IRQ_TIMER_PORT), PIC_DATA_PORT);
}


void init_irq_keyboard() {
    init_irq_entry(KEYBOARD_INTERRUPT_ID, (uint32_t)handler_IT_keyboard);
    outb(inb(PIC_DATA_PORT)&~(1 << IRQ_KEYBOARD_PORT), PIC_DATA_PORT);
}