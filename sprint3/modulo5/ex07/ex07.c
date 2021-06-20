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

#define NUMBER_THREADS 16
#define SIZE 8000
#define NUMBER_CHOICES 5
#define MAX_NUMBERS 49
#define INCREMENT 500

int vec[SIZE][NUMBER_CHOICES];
pthread_mutex_t mutex;
int vec_res[MAX_NUMBERS];

typedef struct {
    int index;
} data_f;

data_f idx;

void* func(void* arg) {
    if (pthread_mutex_lock(&mutex) != 0) {
        perror("Error locking mutex.\n");
    }

    int index = idx.index;

    int start = index * INCREMENT;
    int end = start + INCREMENT;

    //Faz a contagem dos números 
    for (int i = start; i < end; i++) {
        for (int j = 0; j < NUMBER_CHOICES; j++) {
            vec_res[vec[i][j]]++;
        }
    }

    idx.index++;

    if (pthread_mutex_unlock(&mutex) != 0) {
        perror("Error unlocking mutex.\n");
    }

    pthread_exit((void*) NULL);
}

int main() {
    pthread_t threads[NUMBER_THREADS];

    //Preenche o array nas 8000 posições com 5 valores aleatórios entre 1 e 49
    for (int i = 0; i < SIZE; i++) {
        int count = 0;
        for (int j = 0; j < MAX_NUMBERS && count < NUMBER_CHOICES; j++) {
            int random = (rand() % 49) + 1;
            vec[i][count] = random;
            count ++;
        }
    }

    pthread_mutex_init(&mutex, NULL);

    //Cria-se as 16 threads
    for (int i = 0; i < NUMBER_THREADS; i++) {
        pthread_create(&threads[i], NULL, func, NULL);
    }

    //Espera pelo término das 16 threads
    for (int i = 0; i < NUMBER_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    //Mostra as estatísticas
    for (int i = 0; i < MAX_NUMBERS; i++) {
        printf("O número %d foi escolhido %d vezes.\n", i+1, vec_res[i+1]);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}