#ifndef __PROCESSUS_H__
#define __PROCESSUS_H__
#include <inttypes.h>
#include <string.h>
#include "../stdio.h"
#include <n7OS/cpu.h>
#include <n7OS/file.h>

#define NB_PROCS 255

#define STACK_SIZE 1024

typedef uint32_t pid_t;

typedef enum proc_state { FREE, STUCK, READY, ELECTED } proc_state_t;

void init_processus();

pid_t create_process(void * function);

void run_process(void (*function)(void), pid_t pid);

void block_process(pid_t pid);

void terminate_process(pid_t pid);

void schedule();

void function1();

void function2();

#endif