#ifndef __FILE_H__
#define __FILE_H__

#define FILE_LEN 1024

#include <inttypes.h>
typedef uint32_t pid_t;

typedef struct file{
    pid_t processus[FILE_LEN];
    int start;
    int finish;
} proc_file_t;

void init_file(proc_file_t *file);

int enfiler(proc_file_t *file, pid_t pid);

int defiler(proc_file_t *file, pid_t *pid);

void detruire(proc_file_t *file, pid_t pid);

#endif