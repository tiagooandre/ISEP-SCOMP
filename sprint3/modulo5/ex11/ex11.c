#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NUMBER_THREADS 5
#define NUMBER_PROVAS 300
#define FLAG_FINISH_PROVA 0

pthread_mutex_t mutexT1;
pthread_mutex_t mutexT2T3;

typedef struct {
    int number; //student number
    int notaG1;
    int notaG2;
    int notaG3;
    int notaFinal;
} Prova;

Prova provas[NUMBER_PROVAS];

void* generateProva (void *arg) {
    srand(time(NULL));
    for (int i = 0; i < NUMBER_PROVAS; i++) {
        if (pthread_mutex_lock(&mutexT1) != 0) {
            perror("Error locking mutexT1.\n");
        }

        provas[i].number = i;
        provas[i].notaG1 = (rand() % 100);
        provas[i].notaG2 = (rand() % 100);
        provas[i].notaG3 = (rand() % 100);

        if (pthread_mutex_unlock(&mutexT1) != 0) {
            perror("Error unlocking mutexT1.\n");
        }
    }
    
    pthread_exit((void*) NULL);
}

void* calcular_notaF(void* arg) {

    if (pthread_mutex_lock(&mutexT2T3) != 0) {
        perror("Error locking mutexT2T3.\n");
    }

    for (int i = 0; i < NUMBER_PROVAS; i++) {
        provas[i].notaFinal = (provas[i].notaG1 + provas[i].notaG2 + provas[i].notaG3) / 3;
    }

    if (pthread_mutex_unlock(&mutexT2T3) != 0) {
        perror("Error locking mutexT2T3.\n");
    }

    pthread_exit((void*) NULL);
}

int pos = 0;
void* notas_positivas(void *arg) {
    for (int i = 0; i < NUMBER_PROVAS; i++) {
        if (provas[i].notaFinal >= 50) {
            pos++;
        }
    }

    pthread_exit((void*) NULL);
}

int neg = 0;
void* notas_negativas(void* arg) {
    for (int i = 0; i < NUMBER_PROVAS; i++) {
        if (provas[i].notaFinal < 50) {
            neg++;
        }
    }

    pthread_exit((void*) NULL);
}

int main() {
    pthread_t T1;
    pthread_t T2T3[2];
    pthread_t T4;
    pthread_t T5;

    pthread_mutex_init(&mutexT1, NULL);
    pthread_mutex_init(&mutexT2T3, NULL);
    
    for (int i = 0; i < NUMBER_PROVAS; i++) {
        pthread_create(&T1, NULL, generateProva, NULL);
    }

    pthread_join(T1, NULL);

    //Calcular notas finais
    for (int i = 0; i < 2; i++) {
        pthread_create(&T2T3[i], NULL, calcular_notaF, NULL);
    }

    pthread_join(T2T3[0], NULL);
    pthread_join(T2T3[1], NULL);

    for (int i = 0; i < NUMBER_PROVAS; i++) {
        printf("Número estudante: %d\n", provas[i].number);
        printf("Nota final: %d\n", provas[i].notaFinal);
    }

    //Calcula notas positivas
    pthread_create(&T4, NULL, notas_positivas, (void*) NULL);
    
    //Calcula notas negativas
    pthread_create(&T5, NULL, notas_negativas, (void*) NULL);

    pthread_join(T4, NULL);
    pthread_join(T5, NULL);

    double res_pos = (pos * 100) / NUMBER_PROVAS;
    double res_neg = (neg * 100) / NUMBER_PROVAS;

    printf("\nPercentagens: \n");
    printf("Notas positivas: %.2f%%\n", res_pos);
    printf("Notas negativas: %.2f%%\n", res_neg);

    
    pthread_mutex_destroy(&mutexT1);
    pthread_mutex_destroy(&mutexT2T3);

    exit(0);
}