#ifndef __IRQ_H__
#define __IRQ_H__

#include <inttypes.h>
#include <n7OS/segment.h>
#include <n7OS/processor_structs.h>

#define PIC_COMMAND_PORT 0x20
#define PIC_DATA_PORT 0x21

#define IRQ_TIMER_PORT 0
#define TIMER_INTERRUPT_ID 0x20

#define IRQ_KEYBOARD_PORT 1
#define KEYBOARD_INTERRUPT_ID 0x21

/*
Une entrée dans l'IDT est sur 64 bits

Partie poids forts
Bit :     | 31              16 | 15 | 14 13 | 12 | 11 10 9 8 | 7 6 5 | 4 3 2 1 0 |
Contenu : | offset supérieur   | P  | DPL   | S  | GateType  | 0 0 0 |  réservé  |

Partie poids faible
Bit :     | 31              16 | 15              0 |
Contenu : |sélecteur de segment| offset inférieur  |
*/

typedef struct {
  uint16_t offset_inf;
  uint16_t sel_segment;
  uint8_t zero;
  uint8_t type_attr;
  uint16_t offset_sup;
} idt_entry_t;

void handler_timer();
void init_irq_entry(int irq_num, uint32_t addr);
void init_irq();
void init_irq_timer();
void init_irq_keyboard();

#endif
