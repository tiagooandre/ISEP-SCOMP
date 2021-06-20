#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

int main(){

    pid_t pid, cpid;
    int r1, r2, status;
    char m1[] = "Hello world";
    char m2[] = "Goodbye!";
    int tamanhoS1 = strlen(m1) + 1;
    int tamanhoS2 = strlen(m2) + 1;
    int fd[2];
    pipe(fd);
    pid = fork();
	
	if(pid < 0){
        perror("erro no fork");
        exit(1);
    }
    if(pid == 0){
        close(fd[1]);		
		char str1[tamanhoS1];	
		char str2[tamanhoS2];	
		r1 = read(fd[0], str1, sizeof(m1));
		r2 = read(fd[0], str2, sizeof(m2));
		printf("Numero de bytes lidos na 1 string: %d \n", r1);
		printf("Numero de bytes lidos na 2 string: %d \n", r2);
		close(fd[0]);		
		exit(0);
	}
    else{
		close(fd[0]);		
		printf("Numero de bytes na 1 string: %ld \n", sizeof(m1));
		printf("Numero de bytes na 2 string: %ld\n\n", sizeof(m2));
		printf("1 mensagem: %s\n", m1);
		printf("2 mensagem: %s\n", m2);
		write(fd[1], m1, sizeof(m1));
		write(fd[1], m2, sizeof(m2));
		close(fd[1]);		
	}

    cpid = wait(&status);
    printf("\nPID do filho e [%d] e saiu com valor = %d.\n\n", cpid, WEXITSTATUS(status));

	return 0;
}