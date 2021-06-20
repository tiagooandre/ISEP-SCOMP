#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>

int main() {
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

        if (p == 0) {
            printf("Order: %d", i+1);
            exit(i+1);
        }
    }

    printf("This is the end.\n");
}