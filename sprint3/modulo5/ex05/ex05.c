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

#define SIZE 1000
#define NUMBER_THREADS 3

int vec[SIZE], lowest_bal, highest_bal, average_bal; //Variáveis globais

void* lowest_balance() {
    int min = 99999;
    for (int i = 0; i < SIZE; i++) {
        if (vec[i] < min) {
            min = vec[i];
        }
    }
    lowest_bal = min;
    pthread_exit((void*) NULL);
}

void* highest_balance() {
    int max = -99999;
    for (int i = 0; i < SIZE; i++) {
        if (vec[i] > max) {
            max = vec[i];
        }
    }
    highest_bal = max;
    pthread_exit((void*) NULL);
}

void* average_balance() {
    int avg = 0;
    for (int i = 0; i < SIZE; i++) {
        avg += vec[i];
    }
    average_bal = avg / SIZE;
    pthread_exit((void*) NULL);
}

int main() {
    pthread_t threads[NUMBER_THREADS];
    
    //Simular o balanço da conta de um cliente
    for (int i = 0; i < SIZE; i++) {
        vec[i] = i + 100; //Valores entre 100 e 1099
    }

    //Cria-se as 3 threads
    pthread_create(&threads[0], NULL, lowest_balance, NULL);
    pthread_create(&threads[1], NULL, highest_balance, NULL);
    pthread_create(&threads[2], NULL, average_balance, NULL);

    //Espera pelo término das 3 threads
    for (int i = 0; i < NUMBER_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("O balanço mais baixo foi: %d\nO balanço mais alto foi: %d\nO balanço médio foi: %d", lowest_bal, highest_bal, average_bal);
}