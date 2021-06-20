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
    char rand_array[size];
} shared_data_type;

int main() {
    //Criar array com valores aleatórios
    srand(time(NULL));
    int aux[size];
    for (int i = 0; i < size; i++) {
        aux[i] = rand();
    }

    printf("Pipes:\n");
    int fd[2];

    if(pipe(fd) == -1) {
        perror("Pipe failed.\n");
        return 1;
    }

    clock_t start = clock();
    clock_t total;

    pid_t p = fork();

    if (p < 0) {
        perror("Fork failed.\n");
        return 1;
    }

    if (p == 0) {
        close(fd[1]);
        for (int i = 0; i < size; i++) {
            read(fd[0], &aux[i], sizeof(aux[i]));
        }
        close(fd[0]);
        exit(0);
    }

    close(fd[0]);
    int aux_read[size];
    for (int i = 0; i < size; i++) {
        write(fd[1], &aux_read[i], sizeof(aux_read[i]));
    }
    close(fd[1]);

    total = clock() - start;
    float total_t = (float)(total) / CLOCKS_PER_SEC;
    printf("Tempo para copiar o array em Shared Memory: %f seg.\n", total_t);
}