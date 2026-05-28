#include <n7OS/processus.h>

extern void ctx_sw(void *old_ctx, void *new_ctx);

typedef struct processus{
    pid_t pid; // id du processus
    uint32_t regs[5]; // registres du processus
    uint32_t proc_stack[STACK_SIZE]; // stack du processus
    proc_state_t state; // état du processus
} processus_t;


processus_t proc_table[NB_PROCS]; // table des processus

proc_file_t file_proc;

pid_t current_pid;

void init_processus() {
    memset(proc_table, 0, NB_PROCS * sizeof(processus_t));
    init_file(&file_proc);
    current_pid = 0;
    proc_table[0].state = READY;
}

pid_t find_free_pid() {
    for (int i = 0; i < NB_PROCS ; i++) {
        if (proc_table[i].state == FREE) {
            return i;
        }
    }
    return -1;
}

pid_t create_process(void * function) {
    pid_t new_pid = find_free_pid();
    
    if (new_pid < 0) {
        return new_pid;
    }

    processus_t* new_proc = proc_table + new_pid; // création du nouveau processus dans la table à l'indice de son pid
    new_proc->pid = new_pid; // on assigne le pid au processus
    new_proc->regs[1] = (uint32_t) (new_proc->proc_stack + STACK_SIZE - 4); // adresse de retour
    new_proc->proc_stack[STACK_SIZE - 4] = (uint32_t)run_process;
    new_proc->proc_stack[STACK_SIZE - 2] = (uint32_t)function;
    new_proc->proc_stack[STACK_SIZE - 1] = (uint32_t) new_pid;
    new_proc->state = READY;

    enfiler(&file_proc, new_pid);

    return new_proc->pid;
}

void block_process(pid_t pid) {
    if (proc_table[pid].state == READY) {
        proc_table[pid].state = STUCK;
        detruire(&file_proc, pid);
        if (pid == current_pid) {
            schedule();
        }
    } 
}

void unblock_process(pid_t pid) {
    if (proc_table[pid].state == STUCK) {
        proc_table[pid].state = READY;
        enfiler(&file_proc,pid);
    }
}

void terminate_process(pid_t pid) {
    memset(proc_table + pid, 0, sizeof(processus_t));
}

void schedule() {
    pid_t old_pid = current_pid;
    if (proc_table[old_pid].state == ELECTED) {
        proc_table[old_pid].state = READY;
        enfiler(&file_proc, old_pid);
    }
    if (defiler(&file_proc,&current_pid)) {
        proc_table[current_pid].state = ELECTED;
        ctx_sw(proc_table[old_pid].regs, proc_table[current_pid].regs);
    } else {
        current_pid = 0;
        proc_table[current_pid].state = READY;
        ctx_sw(proc_table[old_pid].regs,proc_table[current_pid].regs);
    }

}

void run_process(void (*function) (void), pid_t pid) {
    function();
    proc_table[pid].state = FREE;
    schedule();
}

void function1() {
    printf("Ding\n");
}

void function2() {
    printf("dong\n");
}
