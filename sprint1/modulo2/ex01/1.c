#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>


int main (){

    pid_t pid;
    int LB, ppid;
    int fd[2];
    pipe(fd);
    pid = fork();
    
    
    if(pid < 0){
        perror("erro no fork");
        exit(1);
    }

    if (pid == 0){
       close(fd[1]);
		LB = read(fd[0],&ppid , sizeof(int));
		printf("Numero bytes lido: %d\n", LB);
		printf("PID lido: %d\n\n", ppid);
		close(fd[0]);
    }

    else {
       close(fd[0]);
		ppid = getpid();
		printf("Numero bytes escritos: %ld\n", sizeof(ppid));
		printf("PID escritos: %d\n\n", ppid);
		write(fd[1],&ppid, sizeof(int));
		close(fd[1]);
        
    }


}