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

typedef struct {
	int nrLeitores;
    int nrEscritores;
    char frase[50];
} shared_data_type;

int main(void) {
    sem_t *Escritor;
    sem_t *Ler;
    sem_t *Escrever;
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

    if((Escritor = sem_open("sem1", O_CREAT)) == SEM_FAILED){
        printf("erro1");
    }
    if((Ler = sem_open("sem2", O_CREAT)) == SEM_FAILED){
        printf("erro2");
    }
    if((Escrever = sem_open("sem3", O_CREAT)) == SEM_FAILED){
        printf("erro3");
    }

    sem_wait(Escritor);
    shared_data->nrLeitores++;
    sem_post(Escritor);

    sem_wait(Ler);
    printf("%s\n", shared_data->frase);
    printf("Numero de readers: %d\n", shared_data->nrLeitores);
    sleep(10);
    sem_wait(Escritor);
    shared_data->nrLeitores--;
    if (shared_data->nrLeitores == 0){
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