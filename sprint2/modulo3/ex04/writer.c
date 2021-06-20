#include <sys/mman.h> //shm_open, shm_unlink
#include <sys/stat.h> //Constants used for opening: S_IRUSR, S_IWUSR
#include <fcntl.h> //File Control: O_RDWR, O_CREAT, O_EXCL
#include <unistd.h> //ftruncate
#include <sys/types.h> //ftruncate
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
    char rand_num[100];
} shared_data_type;

int main() {
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    shm_unlink("/ex04"); //Removes the memory area from the file system

    fd = shm_open("/ex04", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //Creates and opens a shared memory area
    ftruncate(fd, data_size); //Defines the size
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //Maps the shared area in the process address space

    printf("WRITER:\n");
    srand(time(NULL));
    char aux[100];
    for (int i = 0; i < 100; i++) {
        aux[i] = (rand() % 26) + 65;            
    }

    strcpy(shared_data->rand_num, aux);

    munmap(shared_data, data_size);
    close(fd);
}