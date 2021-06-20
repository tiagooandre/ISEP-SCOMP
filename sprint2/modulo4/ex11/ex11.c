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
    sem_unlink("/sem_ex11_lot");
    sem_unlink("/sem_ex11_p1");
    sem_unlink("/sem_ex11_p2");
    sem_unlink("/sem_ex11_p3");

    int capacidade_restante;
    sem_t *lot = sem_open("/sem_ex11_lot", O_CREAT|O_EXCL, 0644, 200);
    sem_t *p1 = sem_open("/sem_ex11_p1", O_CREAT|O_EXCL, 0644, 1);
    sem_t *p2 = sem_open("/sem_ex11_p2", O_CREAT|O_EXCL, 0644, 1);
    sem_t *p3 = sem_open("/sem_ex11_p3", O_CREAT|O_EXCL, 0644, 1);

    if (p1 == SEM_FAILED) {
        perror("Error p1: sem_open()");
        exit(-1);
    }

    if (p2 == SEM_FAILED) {
        perror("Error p2: sem_open()");
        exit(-1);
    }

    if (p3 == SEM_FAILED) {
        perror("Error p3: sem_open()");
        exit(-1);
    }

    if (lot == SEM_FAILED) {
        perror("Error lot: sem_open()");
        exit(-1);
    }

    pid_t p;
    int entrada, saida;
    for (int i = 0; i < 200; i++) {
        p = fork(); //200 passageiros

        if (p < 0) {
            perror("Fork failed.\n");
            exit(-1);
        }

        if (p == 0) {
            entrada = 0;
            while (entrada  == 0) {
                for (int i = 0; i < 3; i++) {
                    if (sem_trywait(p1) != 0) {
                        sem_wait(lot);

                        sem_getvalue(lot, &capacidade_restante);
                        printf("Entrada de um passageiro na porta 1. Restantes lugares: %d.\n", capacidade_restante);
                        sem_post(p1);

                        entrada = 1;
                        break;
                    }

                    if (sem_trywait(p2) != 0) {
                        sem_wait(lot);

                        sem_getvalue(lot, &capacidade_restante);
                        printf("Entrada de um passageiro na porta 2. Restantes lugares: %d.\n", capacidade_restante);
                        sem_post(p2);

                        entrada = 1;
                        break;
                    }

                    if (sem_trywait(p3) != 0) {
                        sem_wait(lot);

                        sem_getvalue(lot, &capacidade_restante);
                        printf("Entrada de um passageiro na porta 1. Restantes lugares: %d.\n", capacidade_restante);
                        sem_post(p3);

                        entrada = 1;
                        break;
                    }
                }
            }

            saida = 0;
            while (saida  == 0) {
                for (int i = 0; i < 3; i++) {
                    if (sem_trywait(p1) != 0) {
                        sem_post(lot);

                        sem_getvalue(lot, &capacidade_restante);
                        printf("Saída de um passageiro na porta 1. Restantes lugares: %d.\n", capacidade_restante);
                        sem_post(p1);

                        saida = 1;
                        break;
                    }

                    if (sem_trywait(p2) != 0) {
                        sem_post(lot);

                        sem_getvalue(lot, &capacidade_restante);
                        printf("Saída de um passageiro na porta 2. Restantes lugares: %d.\n", capacidade_restante);
                        sem_post(p2);

                        saida = 1;
                        break;
                    }

                    if (sem_trywait(p3) != 0) {
                        sem_post(lot);

                        sem_getvalue(lot, &capacidade_restante);
                        printf("Saída de um passageiro na porta 1. Restantes lugares: %d.\n", capacidade_restante);
                        sem_post(p3);

                        saida = 1;
                        break;
                    }
                }
            }
            exit(0);
        }
    }
    
    for (int i = 0; i < 200; i++) {
        wait(NULL);
    }

    sem_close(lot);
    sem_close(p1);
    sem_close(p2);
    sem_close(p3);

    sem_unlink("/sem_ex11_lot");
    sem_unlink("/sem_ex11_p1");
    sem_unlink("/sem_ex11_p2");
    sem_unlink("/sem_ex11_p3");
}