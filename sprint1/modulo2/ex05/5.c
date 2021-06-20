#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

#define tamanho_maximo 256


void converter_string(char *string) {
    while (*string) {
        if (*string >= 'A' && *string <= 'Z') {
            *string = *string + 32;
        }
        else if (*string >= 'a' && *string <= 'z') {
            *string = *string - 32;
        }
        string++;
    }
}



int main() {

    int fd[2], fd2[2];
    int status;
    pid_t pid;

    char teclado[tamanho_maximo];
    char ecra[tamanho_maximo];
    
    pipe(fd);
    pipe(fd2);

     pid = fork();

    if (pid < 0){
			perror("Erro no fork");
			exit(1);
			
		} 

     if (pid == 0) {
		close(fd[0]);

		printf("Escreva uma expressão:\n");
		scanf("%[^\n]s", teclado);

		write(fd[1], teclado, strlen(teclado) + 1);
		close(fd[1]);

		close(fd2[1]);
		read(fd2[0], teclado, tamanho_maximo);
		close(fd2[0]);

		printf("Nova palavra: %s\n", teclado);
        
		exit(0);
    }
    else {
		close(fd[1]);
		read(fd[0], ecra, tamanho_maximo);
		close(fd[0]);

		converter_string(ecra);
        
		close(fd2[0]);
		write(fd2[1], ecra, strlen(ecra) + 1);
		close(fd2[1]);

		waitpid(pid, &status, 0);
    } 

    return 0;
}