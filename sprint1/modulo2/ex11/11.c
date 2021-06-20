#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>


int main(){

    int n_processosf = 5;
    int fd[6][2];
    pid_t pid;
    int num_gerado;
    int num_recebido;

    for(int i = 0; i < 6; i++){
        pipe(fd[i]);
    }
    for(int i = 0; i < n_processosf; i++){
        pid = fork();
        if(pid == 0){
            close(fd[i][1]);
            srand(time(NULL) + getpid());
           num_gerado = 1 + rand() % 500;
           printf("O numero %d foi gerado pelo processo %d \n", num_gerado, getpid());
            read(fd[i][0], &num_recebido, sizeof(int));
            close(fd[i][0]);
           
        if(num_gerado > num_recebido){
				write(fd[i+1][1], &num_gerado, sizeof(int));  
            }
        else{
				write(fd[i+1][1], &num_recebido, sizeof(int));
            }
        
        exit(0);
        }

    }
    

     srand(time(NULL) + getpid());
    num_gerado = 1 + rand() % 500;
    printf("O numero %d foi gerado pelo processo %d \n", num_gerado, getpid());

    close(fd[0][0]);
    write(fd[0][1], &num_gerado, sizeof(int));
    close(fd[0][1]);

    close(fd[5][1]);
    read(fd[5][0], &num_gerado,sizeof(int));
    close(fd[5][0]);
    printf("O maior numero e %d \n", num_gerado);


}