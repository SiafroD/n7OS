#include <n7OS/processus.h>
#include <n7OS/file.h>


void init_file(proc_file_t *file) {
    file->start=-1;
    file->finish=-1;
}

int enfiler(proc_file_t *file, pid_t pid) {
    if ((file->finish + 1)% FILE_LEN == (file->start - 1)) {
        return 0; // la file est pleine
    }

    int new_finish = (file->finish + 1) % FILE_LEN;

    file->processus[new_finish] = pid;
    file->finish = new_finish;

    file->start = (file->start == -1) ? new_finish : file->start;

    return 1;
}

int defiler(proc_file_t *file, pid_t *pid) {
    if (file->start == -1) {
        return 0; // la file est vide
    }

    *pid = (file->processus)[file->start];

    if (file->finish != file->start) {
        file->start = (file->start + 1)%FILE_LEN;
    } else {
        init_file(file);
    }
    return 1;

}

void detruire(proc_file_t *file, pid_t pid) {
    int i, j = file->start;
    int offset = 0;
    while (j != file->finish) {
        if (file->processus[i] == pid) {
            offset++;
            j = (i + offset) % FILE_LEN;
            file->processus[i] = file->processus[j];
            if (file->finish == file->start) {
                init_file(file); // on vient de retirer le dernier élément de la liste
            } else {
                file->finish = (file->start - 1) % FILE_LEN;
            }



        }

        else {
            i = (i + 1) % FILE_LEN;
            j = (i + offset) % FILE_LEN;
        }
    }
}