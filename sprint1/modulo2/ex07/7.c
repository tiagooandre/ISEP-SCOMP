#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

#define tam_array 1000
 
int main(){
	
	time_t tempo;
	pid_t pid[5];
	
	int vec1[tam_array];
	int vec2[tam_array];
	int result[tam_array];
	int tabela [5][2];	
	int status = 0, a, b, aux1, aux2;
	srand ((unsigned) time (&tempo));

	for (a = 0; a < tam_array; a++){
		vec1[a] = rand() % 100;			
		vec2[a] = rand() % 100;
	}

	for (a = 0; a < 5; a++){
		pipe(tabela[a]);
		pid[a] = fork();

		if (pid[a] < 0){
			perror("Erro no fork");
			exit(1);
			
		} 
		if (pid[a] == 0){
			close(tabela[a][0]);
			
			int start = 200 * a;		
			int end = (a+1) * 200;
			
			for (b = start; b < end; b++){
				aux1 = vec1[b] + vec2[b];
				write(tabela[a][1], &aux1, sizeof(int));
			}
			close(tabela[a][1]);
			exit(1);
			
		} else { 
			wait(&status);	
			close(tabela[a][1]);
			
			int start = 200 * a;		
			int end = (a+1) * 200;
			
			for(b = start; b < end; b++){
				read(tabela[a][0], &aux2, sizeof(int));
				result[b] = aux2;
				printf("resultado[%d] = %d\n", b, result[b]);	
			}
			close(tabela[a][0]);
		}
	}
	return 0;
}