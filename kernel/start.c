#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <stdio.h>
#include <n7OS/paging.h>
#include <n7OS/mem.h>
#include <n7OS/irq.h>
#include <n7OS/time.h>
#include <unistd.h>
#include <n7OS/sys.h>


void kernel_start(void)
{
    time_t time;
    uint8_t init_time = 0;

    init_console();
    
    initialise_paging();

    init_irq_timer();

    init_timer();

    init_syscall();

    console_puttime();


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
        hlt();
    }
}
