#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <stdio.h>
#include <n7OS/paging.h>
#include <n7OS/mem.h>

void kernel_start(void)
{
    init_console();
    
    initialise_paging();


    //print_mem();


    // lancement des interruptions
    sti();

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
