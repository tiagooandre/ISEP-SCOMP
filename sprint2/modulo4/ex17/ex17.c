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
    sem_unlink("/sem_capacidade");
    sem_unlink("/sem_vip");
    sem_unlink("/sem_special");
    sem_unlink("/sem_normal");

    sem_t *sem_capacidade = sem_open("/sem_capacidade", O_CREAT|O_EXCL, 0644, 300);
    sem_t *sem_vip = sem_open("/sem_vip", O_CREAT|O_EXCL, 0644, 0);
    sem_t *sem_special = sem_open("/sem_special", O_CREAT|O_EXCL, 0644, 0);
    sem_t *sem_normal = sem_open("/sem_normal", O_CREAT|O_EXCL, 0644, 0);

    if (sem_capacidade == SEM_FAILED) {
        perror("Error sem_espera: sem_open()");
        exit(-1);
    }

    if (sem_vip == SEM_FAILED) {
        perror("Error sem_vip: sem_open()");
        exit(-1);
    }

    if (sem_special == SEM_FAILED) {
        perror("Error sem_special: sem_open()");
        exit(-1);
    }

    if (sem_normal == SEM_FAILED) {
        perror("Error sem_normal: sem_open()");
        exit(-1);
    }

    pid_t p;
    int id;

    for (int i = 0; i < 300; i++) {
        p = fork();

        if (p < 0) {
            perror("Fork failed.\n");
            exit(-1);
        }

        id = rand() % 3;
        int capacidade = 0, fila_vip = 0, fila_special = 0, fila_normal = 0;

        if (p == 0) {
            if (sem_getvalue(sem_capacidade, &capacidade) == 0) {
                if (id == 0) {
                    fila_vip++;
                } else if (id == 1) {
                    fila_special++;
                } else {
                    fila_normal++;
                }
            }

            sem_wait(sem_capacidade);

            if (id == 0) {
                sem_post(sem_vip);
                sem_wait(sem_vip);

                printf("Entrou um visitante VIP: %d.\n", i+1);
            }

            if (id == 1) {
                sem_post(sem_special);
                sem_wait(sem_special);

                printf("Entrou um visitante Special: %d.\n", i+1);
            }

            if (id == 2) {
                sem_post(sem_normal);
                sem_wait(sem_normal);

                printf("Entrou um visitante Normal: %d.\n", i+1);
            }

            sleep(5); //Tempo a assistir

            sem_post(sem_capacidade); //Saída
            printf("Saiu visitante: %d\n", i+1);

            if (fila_vip > 0) {
                sem_post(sem_vip);
                fila_vip--;
            } else if (fila_vip <= 0 && fila_special > 0) {
                sem_post(sem_special);
                fila_special--;
            } else if (fila_vip <= 0 && fila_special <= 0 && fila_normal > 0) {
                sem_post(sem_normal);
                fila_normal--;
            }

            exit(0);
        }
    }

    for (int i = 0; i < 300; i++) {
        wait(NULL);
    }

    sem_close(sem_capacidade);
    sem_close(sem_vip);
    sem_close(sem_special);
    sem_close(sem_normal);

    sem_unlink("/sem_capacidade");
    sem_unlink("/sem_vip");
    sem_unlink("/sem_special");
    sem_unlink("/sem_normal");
}