#include <sys/mman.h> //shm_open, shm_unlink
#include <sys/stat.h> //Constants used for opening: S_IRUSR, S_IWUSR
#include <fcntl.h> //File Control: O_RDWR, O_CREAT, O_EXCL
#include <unistd.h> //ftruncate
#include <sys/types.h> //ftruncate
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define STR_SIZE 50
#define NR_DISC 10

typedef struct {
    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
    int sem;
} shared_data_type;

int main() {
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    shm_unlink("/ex12"); //Removes the memory area from the file system

    fd = shm_open("/ex12", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); //Creates and opens a shared memory area
    ftruncate(fd, data_size); //Defines the size
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //Maps the shared area in the process address space

    shared_data->sem = 1;
    int min = 999, max = 0, sum = 0;
    double avg = 0;
    int num;

    pid_t p;
    p = fork();

    if (p < 0) {
        perror("Fork failed.\n");
        exit(-1);
    }

    if (p > 0) {
        printf("Número estudante: \n");
        scanf("%d", &shared_data->numero);

        printf("Nome estudante: \n");
        scanf("%s", &shared_data->nome);

        printf("Notas do estudante: %d\n", NR_DISC);
        for (int i = 0; i < NR_DISC; i++) {
            printf("%d. ", i + 1);
            scanf("%d", &shared_data->disciplinas[i]);
        }
        shared_data->sem = 0;
        wait(NULL);
    } else {
        while (shared_data->sem) {
            ;
        }

        for (int i = 0; i < NR_DISC; i++) {
                if (shared_data->disciplinas[i] < min) {
                    min = shared_data->disciplinas[i];
                }
                if (shared_data->disciplinas[i] > max) {
                    max = shared_data->disciplinas[i];
                }
                sum += shared_data->disciplinas[i];
            }

        avg = ((double)sum)/NR_DISC;

        printf("Número estudante: %d\n", shared_data->numero);
        printf("Nome estudante: %s\n", &shared_data->nome);
        printf("Nota mais baixa: %d\n", min);
        printf("Nota mais alta: %d\n", max);
        printf("Média de todas as notas: %.2f\n", avg);

        exit(0);
    }

    munmap(shared_data, data_size);
    close(fd);
}