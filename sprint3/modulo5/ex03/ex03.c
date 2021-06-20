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

#define SIZE_ARRAY 1000
#define NUMBER_THREADS 10
#define INCREMENT 100
pthread_mutex_t mutex;

int numbers[SIZE_ARRAY];

typedef struct {
	int index;
    int number_search;
    int res;
} data_thread;

data_thread data_t;

void* func(void* arg) {
    if (pthread_mutex_lock(&mutex) != 0) {
        perror("Error locking mutex.\n");
    }

    int index = data_t.index;

    int start = index * INCREMENT;
    int end = start + INCREMENT;

    //Faz a contagem dos números 
    for (int i = start; i < end; i++) {
        if (numbers[i] == data_t.number_search) { //Verificar se o número é encontrado
            printf("O número %d foi encontrado no index %d.\n", data_t.number_search, i);
            printf("A thread que encontrou o número foi a thread %d\n", data_t.index);
        }
    }

    data_t.index++;

    if (pthread_mutex_unlock(&mutex) != 0) {
        perror("Error unlocking mutex.\n");
    }

    pthread_exit((void*) data_t.number_search);

}

int main() {
    for (int i = 0; i < SIZE_ARRAY; i++) { //Preenche array sem duplicados
        numbers[i] = i;
    }

    printf("What's the number to search?\n");
    scanf("%d", &data_t.number_search);

    pthread_mutex_init(&mutex, NULL); //Inicia mutex

    pthread_t threads[NUMBER_THREADS];
    for (int i = 0; i < NUMBER_THREADS; i++) { //Criar 10 threads
        pthread_create(&threads[i], NULL, func, data_t.number_search);
    }

    for (int i = 0; i < NUMBER_THREADS; i++) { //Espera que as 10 threads terminem
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}