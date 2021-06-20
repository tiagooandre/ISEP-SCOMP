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

int main() {
    sem_unlink("/sem_ex01");
    sem_t *sem = sem_open("/sem_ex01", O_CREAT|O_EXCL, 0644, 1); //Semáforo exclusão mútua
    FILE *file_out, *file_in;
    int num;

    if (sem == SEM_FAILED) {
        perror("Error: sem_open()");
        exit(EXIT_FAILURE);
    }

    //Cria, se não existir o ficheiro "Output.txt"
    file_out = fopen("Output.txt", "w");
    fclose(file_out);

    pid_t p;
    for (int i = 0; i < 8; i++) {
        p = fork();

        if (p < 0) {
        perror("Fork failed.\n");
        exit(-1);
        }

        if (p == 0) {
            sem_wait(sem); //Decrementa o semáforo
            file_in = fopen("Numbers.txt", "r");

            if (file_in != NULL) {
                file_out = fopen("Output.txt", "a");

                for (int i = 0; i < 200; i++) {
                    fscanf(file_in, "%d", &num);
                    fprintf(file_out, "%d\n", num);
                }
                fclose(file_out);
                fclose(file_in);
            }
            sem_post(sem); //Incrementa o semáforo
            exit(0);
        }
    }

    for (int i = 0; i < 8; i++) {
        wait(NULL);
    }

    file_out = fopen("Output.txt", "r");
    if (file_out == NULL) {
        printf("Couldn't open the file");
    } else {
        while (fscanf(file_out, "%d", &num) != EOF) {
            printf("%d\n", num);
        }
    }

    fclose(file_out);

    sem_close(sem);
    sem_unlink("/sem_ex01");
}