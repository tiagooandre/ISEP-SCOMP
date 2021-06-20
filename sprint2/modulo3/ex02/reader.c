#include <sys/mman.h> //shm_open, shm_unlink
#include <sys/stat.h> //Constants used for opening: S_IRUSR, S_IWUSR
#include <fcntl.h> //File Control: O_RDWR, O_CREAT, O_EXCL
#include <unistd.h> //ftruncate
#include <sys/types.h> //ftruncate
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int prod_code;
    char description[100];
    float price;
} shared_data_type;

int main_r() {
    int fd, data_size = sizeof(shared_data_type), r;
    shared_data_type *shared_data;

    fd = shm_open("/ex02", O_RDWR, S_IRUSR|S_IWUSR); //Creates and opens a shared memory area
    ftruncate(fd, data_size); //Defines the size
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0); //Maps the shared area in the process address space

    printf("READER:\n");
    printf("Code: %d\n", shared_data->prod_code);

    printf("Description: %s\n", shared_data->description);

    printf("Price: %f\n", shared_data->price);

    r = munmap(shared_data, data_size); //Disconnects the shared memory area from the process andress space
    if (r < 0) exit(1);

    r = close(fd); //Closes the file descritor returned by shm_open()
    if (r < 0) exit(1);

    shm_unlink("/ex02");

    exit(0); //All descriptors are closed
}