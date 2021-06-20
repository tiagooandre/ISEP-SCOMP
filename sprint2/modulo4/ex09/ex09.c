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
    sem_t *p1 = sem_open("/sem_ex09_p1", O_CREAT|O_EXCL, 0644, 0);
    sem_t *p2 = sem_open("/sem_ex09_p2", O_CREAT|O_EXCL, 0644, 0);

    if (p1 == SEM_FAILED) {
        perror("Error p1: sem_open()");
        exit(-1);
    }

    pid_t p;

    for (int i = 0; i < 2; i++) {
        p = fork();

        if (p < 0) {
            perror("Fork failed.\n");
            exit(-1);
        }

        if (p == 0) {
            if (i == 0) {
                printf("buy_chips();\n");
                sem_post(p2);

                sem_wait(p1);
                printf("eat_and_drink();\n");
                sem_post(p1);

                exit(0);
            }

            if (i == 1) {
                sem_wait(p2);
                printf("buy_beer();\n");
                sem_post(p1);

                sem_post(p1);
                printf("eat_and_drink();\n");
                sem_wait(p1);

                exit(0);
            }
        }
    }

    for (int i = 0; i < 1; i++) {
        wait(NULL);
    }
    
    sem_close(p1);
    sem_close(p2);
    sem_unlink("/sem_ex09_p1");
    sem_unlink("/sem_ex09_p2");
}