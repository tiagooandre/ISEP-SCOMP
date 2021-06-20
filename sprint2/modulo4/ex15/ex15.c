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
    sem_unlink("/sem_espera");
    sem_unlink("/sem_visitantes");
    sem_unlink("/sem_comeca_show");
    sem_unlink("/sem_saida");

    sem_t *sem_espera = sem_open("/sem_espera", O_CREAT|O_EXCL, 0644, 1);
    sem_t *sem_visitantes = sem_open("/sem_visitantes", O_CREAT|O_EXCL, 0644, 5);
    sem_t *sem_comeca_show = sem_open("/sem_comeca_show", O_CREAT|O_EXCL, 0644, 0);
    sem_t *sem_saida = sem_open("/sem_saida", O_CREAT|O_EXCL, 0644, 1);

    if (sem_espera == SEM_FAILED) {
        perror("Error sem_espera: sem_open()");
        exit(-1);
    }

    if (sem_visitantes == SEM_FAILED) {
        perror("Error sem_visitantes: sem_open()");
        exit(-1);
    }

    if (sem_comeca_show == SEM_FAILED) {
        perror("Error sem_comeca_show: sem_open()");
        exit(-1);
    }

    if (sem_saida == SEM_FAILED) {
        perror("Error sem_saida: sem_open()");
        exit(-1);
    }

    pid_t p;

    for (int i = 0; i < 5; i++) {
        p = fork();

        if (p < 0) {
            perror("Fork failed.\n");
            exit(-1);
        }

        if (p == 0) {
            sem_wait(sem_visitantes);
            sem_wait(sem_espera); //Controlar entrada
            sem_post(sem_espera);

            printf("Número de visitantes à espera: %d\n", i+1);

            if (i+1 == 5) {
                sem_post(sem_comeca_show); //inicia show com sala cheia
            }

            sem_wait(sem_comeca_show);
            sem_post(sem_comeca_show);

            printf("Visitante %d está a assistir!\n", i+1);

            sleep(10); //duração do show

            sem_wait(sem_saida);
            sem_post(sem_saida);

            sem_post(sem_visitantes);
            printf("Visitante %d saiu!\n", i+1);

            exit(0);
        }
    }

    for (int i = 0; i < 5; i++) {
        wait(NULL);
    }

    sem_close(sem_espera);
    sem_close(sem_visitantes);
    sem_close(sem_comeca_show);
    sem_close(sem_saida);
    
    sem_unlink("/sem_espera");
    sem_unlink("/sem_visitantes");
    sem_unlink("/sem_comeca_show");
    sem_unlink("/sem_saida");
}