#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>


void preencher_array(int *array, int size){
    int i;
    srand(time(NULL) * getpid());
    for (i = 0; i < size; i++){
		array[i] = (rand() % 20 + 1);
	}
}

int main(){
	
	int fd, size = 10 * sizeof(int), *array;
	fd = shm_open("/shm", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		printf("Erro na criacao da memória partilhada!\n");
		exit(EXIT_FAILURE);
	}
	
	int aux = ftruncate(fd, size);
	if(aux == -1){
		printf("Erro na alocação da memória.\n");
		exit(EXIT_FAILURE);
	}
	
	array = (int*) mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(array == MAP_FAILED){
		printf("Erro na mapeacao da memória.\n");
		exit(EXIT_FAILURE);
	}
	
	preencher_array(array, 10);
	
	return 0;
}