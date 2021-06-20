#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <sys/types.h>
#include <sys/wait.h>

int pid;

void signal_handler (int signum){

    if (signum == SIGUSR1){
    printf("SIGUSR1 has been captured and was sent by the process with PID %d", pid);
    }

}


int main(){
    signal(SIGUSR1, signal_handler);
    pid = getpid();
    
    kill(pid, SIGUSR1);
    

}