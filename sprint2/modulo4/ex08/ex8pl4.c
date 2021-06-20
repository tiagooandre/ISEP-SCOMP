#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

typedef struct{
    int var;
} shared_data;


int main(void) {
    sem_t *sem1;
    sem_t *sem2;
    sem_unlink("sem");
    sem_unlink("semaf");

    sem1 = sem_open("sem", O_CREAT | O_EXCL, 0644, 1);
    sem2 = sem_open("semaf", O_CREAT | O_EXCL, 0644, 0);


    if (sem1 == SEM_FAILED) {
        perror("Erro no sem_open()");
        exit(1);
    }
    if (sem2 == SEM_FAILED) {
        perror("Erro no sem_open()");
        exit(1);
    }
    int fd, r, i = 0, size = sizeof(shared_data);
    shared_data *data;
	fd = shm_open("/shm", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd == -1){
		printf("Erro na criação da memória partilhada!\n");
		return 1;
	}
	if (ftruncate (fd, size) == -1){
        printf("Erro no ftruncate!\n");
        return 1;
    }
	data = (shared_data*)mmap(NULL, size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    data->var = 0;
    pid_t p = fork();
    if (p == -1){
        printf("Erro no fork!");
        return 1;
    }
    if (p > 0){
        while(i < 5){
            sem_wait(sem1);
            while (data->var < 1){
                printf("S\n");
                data->var += 1;
            }
            i++;
            sem_post(sem2);
        }
    } else {
        while (i < 5){
            sem_wait(sem2);
            while (data->var > -1){
                printf("C\n");
                data->var -= 1;
            }
            i++;
            sem_post(sem1);
        }
        exit(0);
    }
    wait(NULL);

    r = munmap(data, size);
    if (r == -1){
        exit(1);
    }
    r = close(fd);
    if (r == -1){
        exit(1);
    }
    return 0;
}