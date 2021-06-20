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
	int num1;
	int num2;
} valores;



int main(void) {
	int fd, i = 0, size = sizeof(valores);
	valores *num;
	
	shm_unlink ("/shm");
	
	fd = shm_open("/shm", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd == -1){
		printf("Erro na criacao da memória partilhada!");
		return 1;
	}
	
	ftruncate (fd, size);
	if (ftruncate (fd, size) == -1) {
        perror("Erro na alocação da memória.");
        return 0;
    }
    
	num = (valores*)mmap(NULL, size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if (num == MAP_FAILED || num == 0) {
        perror("Erro na mapeacao da memória.");
        return EXIT_FAILURE;
    }
    
    num->num1 = 8000;
    num->num2 = 200;
    
    pid_t p = fork();
    
	if (p < 0) {
			perror("Erro na criacao do fork.");
		}
	 if (p > 0) {
		while (i < 1000000){
			num->num1 += 1;
			num->num2 -= 1;
			i++;
		}
	} if (p == 0) {
		while (i < 1000000){
			num->num1 -= 1;
			num->num2 += 1;
			i++;
		}
		exit(0);
    }
    wait(0);
    
    printf("Número 1: %d e o Número 2: %d\n", num->num1, num->num2);
    
	return 0;
}