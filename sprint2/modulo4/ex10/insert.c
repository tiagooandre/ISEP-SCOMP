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
		perror("Erro na abertura\n");
		exit(0);
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
		printf("Erro no sem_open\n");
		return 0;
	}

	sem_wait(sem);

	record *r = &(sd->entry[sd->index]);

	printf("Inserir numero:\n");

	scanf("%d", &(r->number));
	fflush(0);

	printf("Inserir nome:\n");

	char aux[50];
	scanf("%s", aux);
	strcpy(r->name, aux);
	fflush(0);

	printf("Inserir morada:\n");

  char aux2[100];
	scanf("%s", aux2);
	strcpy(r->address, aux2);
	fflush(0);

	sd->index++;

	sem_post(sem);
	
	sem_unlink("/sem");

	munmap(sd, size);
	close(fd);

	return 0;
}