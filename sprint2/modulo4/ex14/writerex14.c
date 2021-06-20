#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>

typedef struct {
	int nrLeitores;
    int nrEscritores;
    char frase[50];
} shared_data_type;

int main(void) {
    sem_t *Escritor;
    sem_t *Ler;
    sem_t *Escrever;
    sem_unlink("sem1");
    sem_unlink("sem2");
    sem_unlink("sem3");
    shm_unlink("/ex14");
    int fd, r, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/ex14", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1){
		printf("Erro na criação da memória partilhada!");
		return 1;
	}
	if (ftruncate (fd, data_size) == -1){
        printf("Erro no ftruncate!");
        return 1;
    }
	shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    if ((Escritor = sem_open("sem1", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Erro no sem_open()");
        exit(1);
    }
    if ((Ler = sem_open("sem2", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Erro no sem_open()");
        exit(1);
    }
    if ((Escrever = sem_open("sem3", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Erro no sem_open()");
        exit(1);
    }
    int i;
    sem_wait(Escritor);
    shared_data->nrEscritores++;
    sem_post(Escritor);
    sem_wait(Escrever);
    time_t timeinfo;
	time(&timeinfo);
    char *time_str = ctime(&timeinfo);
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Meu PID e %d - %s\n", getpid(), time_str);
    strcpy(shared_data->frase, buffer);
    sleep(20);
    sem_wait(Escritor);
    shared_data->nrEscritores--;
    if(shared_data->nrEscritores == 0){
        for (i = 0; i < shared_data->nrLeitores; i++){
            sem_post(Ler);
        }
    } else {
        sem_post(Escrever);
    }
    sem_post(Escritor);

    r = munmap(shared_data, data_size);
    if (r == -1){
        exit(1);
    }
    r = close(fd);
    if (r == -1){
        exit(1);
    }
}