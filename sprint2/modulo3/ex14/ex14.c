#include <sys/mman.h> //shm_open, shm_unlink
#include <sys/stat.h> //Constants used for opening: S_IRUSR, S_IWUSR
#include <fcntl.h> //File Control: O_RDWR, O_CREAT, O_EXCL
#include <unistd.h> //ftruncate
#include <sys/types.h> //ftruncate
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SIZE 10
#define OPERATIONS 30

typedef struct {
    int numbers[SIZE];
    int sem;
} shared_data_type;

int main() {
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    shm_unlink("/ex14"); //Removes the memory area from the file system

    fd = shm_open("/ex14", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //Creates and opens a shared memory area
    ftruncate(fd, data_size); //Defines the size
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //Maps the shared area in the process address space

    shared_data->sem = 1;

    pid_t p;
    p = fork();

    if (p < 0) {
        perror("Fork failed.\n");
        exit(-1);
    }

    if (p > 0) {
        for (int i = 0; i < OPERATIONS; i++) {
            while (shared_data->sem) { //espera que se torne 0
                ;
            }
            for (int j = 0; j < SIZE; j++) {
                printf("%d\n", shared_data->numbers[j]);
            }
        }
        shared_data->sem = 1;
        wait(NULL);
    } else {
        for (int i = 0; i < OPERATIONS; i++) {
            while (!shared_data->sem) {
                ;
            }
            for (int i = 0; i < SIZE; i++) {
                shared_data->numbers[i] = rand() % 101;
            }
        }
        shared_data->sem = 0;
        exit(0);
    }

    munmap(shared_data, data_size);
    close(fd);
}