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
#include <sys/time.h>


void encher_array(int *array, int size){
    int i;
    srand(time(NULL) * getpid());
    for (i = 0; i < size; i++){
		array[i] = (rand() % 1000);
	}
}

void impressao_array(int *array){
	int i;
	size_t length = sizeof(array)/sizeof(array[0]);
	for(i = 0; i < length; i++){
		printf("Array[%d] = %d\n", i, array[i]);
	}
}

long getMicrotime(){	
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

int main(){
	int fd, *array, tabela[2], size = 1000000 * sizeof(int);
	long inicio, fim; 
	pipe(tabela);
	
	shm_unlink ("/shm");
	
	inicio = getMicrotime();
	fd = shm_open("/shm", O_CREAT|O_EXCL|O_RDWR, S_IWUSR|S_IRUSR);
	if(fd == -1){
		printf("Erro na criacao da memoria partilhada\n");
		exit(EXIT_FAILURE);
	}
	
	int aux = ftruncate(fd, size);
	if(aux == -1){
		printf("Erro na alocacao da memoria partilhada!\n");
		exit(EXIT_FAILURE);
	}
	
	array = (int*) mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(array == MAP_FAILED){
		printf("Erro no mapeamento da memoria partilhada!\n");
		exit(EXIT_FAILURE);
	}
	
	pid_t pid = fork();
	
	if(pid > 0){
		close(tabela[1]);
		read(tabela[0], &fim, sizeof(int));
		close(tabela[0]);
		
		double total = (double) (fim - inicio)/CLOCKS_PER_SEC;
		printf("\nTotal time: %f microseconds.\n", total);
		
	} else {
		close(tabela[0]);
		fill_array(array, 1000000);
		
		fim = getMicrotime();
		write(tabela[1], &fim, sizeof(int));
		close(tabela[1]);
        impressao_array(array);
		
		exit(0);
	}
	
	return 0;
}