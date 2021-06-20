#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	pid_t p;

	p = fork();
	if (p < 0) {
		printf("Fork failed.\n");
		exit(0);
	}
	
	if (p == 0) {
        printf("I'm..\n");
    } else {
        wait(NULL);
        printf("I'll never join you!\n");

        p = fork();
        if (p == 0) {
            printf("the..\n");
        } else {
            wait(NULL);
            printf("I'll never join you!\n");

            p = fork();
            if (p == 0) {
                printf("father!\n");
            } else {
                wait(NULL);
                printf("I'll never join you!\n");
            }
        }
    }
    return 0;
}