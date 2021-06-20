#include <sys/mman.h> //shm_open, shm_unlink
#include <sys/stat.h> //Constants used for opening: S_IRUSR, S_IWUSR
#include <fcntl.h> //File Control: O_RDWR, O_CREAT, O_EXCL
#include <unistd.h> //ftruncate
#include <sys/types.h> //ftruncate
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define size 1000000

typedef struct {
    int rand_num[size];
} shared_data_type;

int main() {
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    shm_unlink("/ex06"); //Removes the memory area from the file system

    fd = shm_open("/ex06", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //Creates and opens a shared memory area
    ftruncate(fd, data_size); //Defines the size
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //Maps the shared area in the process address space
    
    printf("Shared Memory:\n");
    //Criar array com valores aleatórios
    srand(time(NULL));
    int aux[size];
    for (int i = 0; i < size; i++) {
        aux[i] = rand();
    }

    clock_t start = clock();
    clock_t total;

    pid_t p;
    p = fork();

    if (p < 0) {
        perror("Fork failed.\n");
        exit(-1);
    }

    if (p == 0) {
        for (int i = 0; i < size; i++) {
            shared_data->rand_num[i] = aux[i];
        }
        exit(0);
    }

    wait(NULL);

    total = clock() - start;
    float total_t = (float)(total) / CLOCKS_PER_SEC;
    printf("Tempo para copiar o array em Shared Memory: %f seg.\n", total_t);

    munmap(shared_data, data_size);
    close(fd);
}