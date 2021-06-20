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


typedef struct{
	int value1;
	int value2;
} shared_data;

int main(){
	int tabela[2];
	pipe(tabela);
	int i, fd, size = sizeof(shared_data);
	shared_data *sd;
	
	shm_unlink ("/shm"); 
	
	fd = shm_open("/shm", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if(fd == -1){
		printf("Erro na criacao da memória partilhada!\n");
		exit(EXIT_FAILURE);
	}
	
	int aux = ftruncate(fd, size);
	if(aux == -1){
		printf("Erro na definicao do tamanho da memoria.\n");
		exit(EXIT_FAILURE);
	}
	
	sd = (shared_data*) mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(sd == MAP_FAILED){
		printf("Erro na mapeacao da memória.\n");
		exit(EXIT_FAILURE);
	}
	
	sd->value1 = 8000;
	sd->value2 = 200;
	
	pid_t pid1 = fork();
	if(pid1 == -1){
		printf("Erro na criacao do fork.");
		exit(EXIT_FAILURE);
	}
    
    if(pid1 > 0){
		for(i = 0; i < 1000000; i++){
			sd->value1++;
			sd->value2--;
		}
	}
    else{
		for(i = 0; i < 1000000; i++){
			sd->value1--;
			sd->value2++;
		}
		exit(0);
    }
    wait(NULL);		//garante que o processor filho nao fique orfao
    
    printf("\nValor 1: %d\nValor 2: %d\n", sd->value1, sd->value2);
    
    pid_t pid2 = fork();	
    int max;
    
    if(pid2 > 0){
		close(tabela[1]);
		read(tabela[0], &max, sizeof(int));
		close(tabela[0]);
	}
    else{
		close(tabela[0]);
		
		if(sd->value1 >= sd->value2){
			max = sd->value1;
		}else{
			max = sd->value2;
		}
		
		write(tabela[1], &max, sizeof(int));
		close(tabela[1]);
		exit(0);
	}
	
	int unm = munmap(sd, size);
    if (unm == -1) {
        printf("Erro no unmap da memoria partilhada!\n");
        exit(EXIT_FAILURE);
    }

    int closeFd = close(fd);
    if (closeFd == -1){
		printf("Erro a fechar o ficheiro!\n");
        exit(EXIT_FAILURE);
	}
    
    printf("\nValor Maximo (lido pelo pai atraves do pipe) = %d\n", max);
    return 0;
}