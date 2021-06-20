#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


void handle_sigint(int sig) {
    printf("I won’t let the process end with CTRL-C!”");
}

int main(int argc, char* argv[]) {
    /*struct sigint sa;
    sa.sa_handler = &handle_sigint;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL); */

    signal(SIGINT, &handle_sigint);
 for(;;){
 printf("I Like Signal\n");
 sleep(1);
 }
}