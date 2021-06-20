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

#define SIZE 10

typedef struct {
    int buffer[SIZE];
    int count;
} shared_data_type;

int main() {
    sem_unlink("/sem_ex13_sem1");
    sem_unlink("/sem_ex13_sem_full");
    sem_unlink("/sem_ex13_sem_control");
    sem_unlink("/sem_ex13_sem_control_2");

    //int count = 0;
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    shm_unlink("/ex13"); //Removes the memory area from the file system

    fd = shm_open("/ex13", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //Creates and opens a shared memory area
    ftruncate(fd, data_size); //Defines the size
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //Maps the shared area in the process address space

    sem_t *sem1 = sem_open("/sem_ex13_sem1", O_CREAT|O_EXCL, 0644, 1);
    sem_t *sem_full = sem_open("/sem_ex13_sem_full", O_CREAT|O_EXCL, 0644, 10);
    sem_t *sem_control = sem_open("/sem_ex13_sem_control", O_CREAT|O_EXCL, 0644, 0);
    sem_t *sem_control_2 = sem_open("/sem_ex13_sem_control_2", O_CREAT|O_EXCL, 0644, 1);

    if (sem1 == SEM_FAILED) {
        perror("Error sem1: sem_open()");
        exit(-1);
    }

    if (sem_full == SEM_FAILED) {
        perror("Error sem_full: sem_open()");
        exit(-1);
    }
    
    if (sem_control == SEM_FAILED) {
        perror("Error sem_control: sem_open()");
        exit(-1);
    }

    if (sem_control_2 == SEM_FAILED) {
        perror("Error sem_control_2: sem_open()");
        exit(-1);
    }

    shared_data->count = 0;

    pid_t p;
    srand(time(NULL));
    int k = 0, l = 0;

    for (int i = 0; i < 2; i++) { //Cria dois novo processos
        p = fork();

        if (p < 0) {
            perror("Fork failed.\n");
            exit(-1);
        }

        if (p == 0) { //Produtor
            sem_wait(sem_control_2);
            for (int j = 0; j < 10; j++) { //Escrever 10 valores
                sem_wait(sem_full);
                sem_wait(sem1);
                shared_data->buffer[j] = shared_data->count;
                sem_post(sem1);
                shared_data->count++;
            }
            sem_post(sem_control);
            exit(0);
        }
    }

    for (int i = 0; i < 2; i++) { //Consumidor
        sem_wait(sem_control);
        sem_wait(sem1);
        for (int i = 0; i < 10; i++) {
            printf("%d\n", shared_data->buffer[i]);
            sem_post(sem_full); //Repôr semaforo para que possa voltar a escrever 10 valores
        }
        sem_post(sem1);
        sem_post(sem_control_2);
    }
    
    munmap(shared_data, data_size);
    close(fd);

    sem_close(sem1);
    sem_close(sem_full);
    sem_close(sem_control);
    sem_close(sem_control_2);

    sem_unlink("/sem_ex13_sem1");
    sem_unlink("/sem_ex13_sem_full");
    sem_unlink("/sem_ex13_sem_control");
    sem_unlink("/sem_ex13_sem_control_2");
}