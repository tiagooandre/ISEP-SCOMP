#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <time.h>
#include <string.h>

int main() {
    sem_t *sem = sem_open("/sem_ex05", O_CREAT|O_EXCL, 0644, 0);

    if (sem == SEM_FAILED) {
        perror("Error: sem_open()");
        exit(-1);
    }

    pid_t p;
    p = fork();

    if (p < 0) {
        perror("Fork failed.\n");
        exit(-1);
    }

    if (p > 0) {
        sem_wait(sem);
        printf("I'm the father\n");
    }

    if (p == 0) {
        printf("I'm the child\n");
        sem_post(sem);
    }
    
    sem_close(sem);
    sem_unlink("/sem_ex05");
}