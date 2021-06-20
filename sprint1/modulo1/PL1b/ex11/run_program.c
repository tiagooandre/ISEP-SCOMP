#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


int main (int argc, char *argv[]) {

    for(int i = 1; i < argc; i++){
    printf("%s ", argv[i]);
    }
    printf("\n");
}