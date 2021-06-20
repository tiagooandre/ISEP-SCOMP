#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
 
int main () {
    
    pid_t p;
    int status;

    for (int i = 0; i < 2; i++) {
        p = fork();

        if (p < 0) {
            perror("Fork failed.\n");
            exit(-1);
        }

        if (p > 0 && p % 2 != 0) {
            wait(&status);
        }
    }

    printf("This is the end.\n");
}
/*
    int i, j, status;
    pid_t Pids[4];
 
    for (i = 0; i < 4; i++) {
        if((Pids[i] = fork ()) == 0) {
            printf("(son) pid %d from (parent) pid %d.\n", getpid(), getppid());
            exit(0);
        }
    }
    for(j = 0; j < 4; j++){
        if(Pids[j] % 2 == 0) {
            printf("Waiting for child process with even PID (%d)\n", Pids[j]);
            waitpid(Pids[j], &status, 0);
            if(WIFEXITED(status)){
                printf("Even PID %d was executed successfully.\n", Pids[j]);
            }
        }
    }
    
    printf ("Parent process.\n");
    return 0;
}*/