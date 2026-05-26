#include <inttypes.h>
#include <n7OS/irq.h>
#include <n7OS/time.h>
#include <n7OS/cpu.h>

extern void handler_IT_timer();

void handler_timer() {
    outb(0x20, PIC_COMMAND_PORT); 
    timer_ticks++;
}


void init_irq_entry(int irq_num, uint32_t addr) {
    idt_entry_t *entry = (idt_entry_t *)&idt[irq_num];
    entry->offset_inf = addr & 0xFFFF;
    entry->zero = 0x00;
    entry->offset_sup = addr >> 16;
    entry->sel_segment = KERNEL_CS; 
    entry->type_attr = 0x8E;

}

void init_irq_timer() {
    init_irq_entry(TIMER_INTERRUPT_ID, (uint32_t)handler_IT_timer);

    outb(inb(PIC_DATA_PORT)&~(1 << IRQ_TIMER_PORT), PIC_DATA_PORT);
}
