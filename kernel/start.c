#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <stdio.h>
#include <n7OS/paging.h>
#include <n7OS/mem.h>
#include <n7OS/irq.h>
#include <n7OS/time.h>
#include <n7OS/unistd.h>
#include <n7OS/sys.h>
#include <n7OS/keyboard.h>
#include <n7OS/processus.h>

/*
IMPLEMENTATIONS :
- console
- pagination
- interruptions (timer et clavier)
- timer
- syscall (shutdown et write)
- clavier
- processus (création de deux processus et ordonnancement à t = 10 secondes)

*/


void kernel_start(void)
{
    time_t time;
    uint8_t init_time = 0;

    init_console();
    
    initialise_paging();

    init_processus();

    init_irq();

    init_timer();

    init_syscall();

    init_keyboard();

    console_puttime();

    create_process(function1);

    create_process(function2);


    //print_mem();


    // lancement des interruptions
    sti();

    //__asm__("int $50");

    // on ne doit jamais sortir de kernel_start
    while (1) {

        time = get_time_from_ticks(timer_ticks);
        if (time.sec != init_time) {
            init_time = time.sec;
            console_puttime();
        }       
        /*
        if (time.sec == 5) {
            shutdown(1);
        }
        */

        uint16_t character = kgetch();

        if (character) {
            switch(character) {
                case KEY_RETURN:
                    printf("\n");
                    break;
                case KEY_BACKSPACE:
                    printf("\b");
                    break;
                default:
                    printf("%c", character);
            }
        }

           

        console_putcurs();

        hlt();
    }
}
