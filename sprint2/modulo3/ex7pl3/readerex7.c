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


void calcular_media(int *array){
	int i, soma = 0, media;
	for(i = 0; i < 10; i++){
		soma += array[i];
		media = soma/10;
	}
	printf("\nMedia = %d\n", media);
}

void impressao_array(int *array){
	printf("\n");
	int i;
	for(i = 0; i < 10; i++){
		printf("Array[%d] = %d\n", i, array[i]);
	}
}

int main(){
	int fd, size = 10 * sizeof(int), *array;
	fd = shm_open("/shm", O_RDWR, S_IRUSR|S_IWUSR);
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
	
	impressao_array(array);
	calcular_media(array);
	
	return 0;
}