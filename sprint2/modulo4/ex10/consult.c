#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>

typedef struct{
	int number;
	char name[50];
	char address[100];
}record;

typedef struct{
	record entry[100];
	int index;
}SharedData;

int main(void){
	int fd = shm_open("/shm", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	if(fd == -1){
    perror("Erro na abertura \n");
		exit(EXIT_FAILURE);
	}

	int size = sizeof(SharedData);

	if(ftruncate(fd, size) == -1){
		perror("Erro no ftruncate\n");
		exit(0);
	}

	SharedData *sd = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(sd == NULL){
		printf("Erro no mmap\n");
		exit(0);
	}
	
	sem_t *sem;
	
	sem = sem_open("/sem", O_CREAT, 0644, 1);
	if(sem == SEM_FAILED){
		printf("Erro\n");
		return 0;
	}

	sem_wait(sem);

	int number;

	printf("Inserir numero:\n");

	scanf("%d", &number);
	fflush(0);

	if(sd->index == 0){
		printf("nenhum registro disponivel.\n");
	}
	else{
		int i;
		for(i = 0; i < sd->index; i++){
			record *r = &(sd->entry[i]);
			if(r->number == number){
				printf("Registro: %d\nNumero: %d  Nome: %s  Morada: %s\n", i+1, r->number, r->name, r->address);
				printf("\n");
			}
		}
	}

	sem_post(sem);
	
	sem_unlink("/sem");

	munmap(sd, size);
	close(fd);

	return 0;
}