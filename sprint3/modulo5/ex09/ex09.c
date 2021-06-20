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

#define NUMBER_THREADS 4
#define NUMBER_STOPS 3

pthread_mutex_t mutex;

void* func(void* arg) {
    srand(time(NULL));
    int count_trip = 0;

    //Cada thread individual
    if (pthread_mutex_lock(&mutex) != 0) {
        perror("Error locking mutex.\n");
    }

    int number_train = (int) arg;

    printf("Train number is: %d\n", number_train);
    int sleep_time = (rand() % 5) + 1;
    sleep(sleep_time);
    count_trip += sleep_time;
    printf("Origin: Station A\n");
    printf("Destination: Station B\n");
    printf("Trip's duration: %d\n\n", sleep_time);

    //Para gerar aleatoriamente o caminho do comboio
    if (sleep_time % 2 == 0) {
        int sleep_time = (rand() % 5) + 1;
        sleep(sleep_time);
        count_trip += sleep_time;
        printf("Origin: Station B\n");
        printf("Destination: Station C\n");
        printf("Trip's duration: %d\n\n", sleep_time);
    } else {
        int sleep_time = (rand() % 5) + 1;
        sleep(sleep_time);
        count_trip += sleep_time;
        printf("Origin: Station B\n");
        printf("Destination: Station D\n");
        printf("Trip's duration: %d\n\n", sleep_time);
    }

    printf("Total trip time: %d\n\n", count_trip);

    if (pthread_mutex_unlock(&mutex) != 0) {
        perror("Error unlocking mutex.\n");
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUMBER_THREADS];

    //Cria-se as 4 threads
    for (int i = 0; i < NUMBER_THREADS; i++) {
        int number_train = i;
        pthread_create(&threads[i], NULL, func, (void*) number_train);
    }

    //Espera pelo término das 4 threads
    for (int i = 0; i < NUMBER_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}