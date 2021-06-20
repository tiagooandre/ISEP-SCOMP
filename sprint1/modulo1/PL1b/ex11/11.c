#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

typedef struct {
 char cmd[32];
 int tempo;
} comando; 

void functioncaller(char cmd[32], int tempo) {
    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork error");
        exit(1);
    }

    if (pid == 0){
       //char *path = './run_program';
       char *args[] = {"./run_program", cmd, NULL};
        execv(args[0], args);
    }
    else {
        sleep(tempo);
        kill(pid, SIGINT);
        printf("Execution ended.");
        
    }
}

int main() {

    

    comando* comandos[3];

    

    comando *a;
    a = malloc(sizeof(comando));
    strncpy(a->cmd, "Programa 1", 100);
    a->tempo = 5;

    comando *b;
    b = malloc(sizeof(comando));
    strncpy(b->cmd, "Programa 2", 100);
    b->tempo = 4;

    comando *c;
    c = malloc(sizeof(comando));
    strncpy(c->cmd, "Programa 3", 100);
    c->tempo = 6;

    

    comandos[0] = a;
    comandos[1] = b;
    comandos[2] = c;
    
    for(int i = 0;i < 3;i++){
        functioncaller(comandos[i]->cmd,comandos[i]->tempo);
    }
    
}