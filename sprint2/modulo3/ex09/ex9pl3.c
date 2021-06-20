#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>




typedef struct{
	int geral[1000];
	int filhos[10];
} valores;



int main(void) {
	
	int fd, ft, size = sizeof(valores);
    valores *numeros;
    	
	shm_unlink ("/shm");
	
	fd = shm_open("/shm", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd == -1) {
		perror("Erro na criacao da memória partilhada!");
		return 1;
	}

	ft = ftruncate (fd, size);
	if (ft == -1) {
        perror("Erro na alocação da memória.");
        return 0;
    }
    
	numeros = (valores*)mmap(NULL, size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if (numeros == MAP_FAILED || numeros == 0) {
        perror("Erro na mapeacao da memória.");
        return EXIT_FAILURE;
    }



    int i, j, maximo_global = 0;
	int status;
    pid_t p[10];
    
    srand(time(NULL));
	for (i = 0; i < 1000; i++) {
		numeros->geral[i] = rand() % 1000;
	}
			
			
    for (i = 0; i < 10; i++) {
		
		int maximo_filho = 0;
		
		if ((p[i] = fork()) == -1) {
			perror("Erro na execução do fork.");
		} 
        if (p[i] == 0) {
			for (j = 100*i; j < (100*i)+99; j++) {
				if (numeros->geral[j] > maximo_filho) {
					maximo_filho = numeros->geral[j];
				}
			}
			numeros->filhos[i] = maximo_filho;
			exit(i);
		}
	}
	
	for (i = 0; i < 10; i++) {
		waitpid(p[i], &status, 0);
		if (numeros->filhos[i] > maximo_global) {
			maximo_global = numeros->filhos[i];
		}
		
	}
	
	printf("Maior valor encontrado: %d \n", maximo_global);
	
	return 0;
}