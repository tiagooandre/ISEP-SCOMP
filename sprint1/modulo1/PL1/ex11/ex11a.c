#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

#define ARRAY_SIZE 1000

int main() {
    int numbers[ARRAY_SIZE], n, i, j, max, status;
    time_t t;
    pid_t p;

    srand ((unsigned) time (&t));

    for (i = 0; i < ARRAY_SIZE; i++) {
        numbers[i] = rand() % (256);
    }

    for (i = 0; i < 5; i++) {
        p = fork();
        
        if (p < 0) {
            perror("Fork failed.\n");
            exit(1);
        }
        
        if (p == 0) {
            max = 0;
            for (j = i * (ARRAY_SIZE/5); j < (i + 1) * (ARRAY_SIZE/5); j++) {
                if (numbers[j] > max) {
                    max = numbers[j];
                }
                exit(max);
            }
        }
    }
    
    for (i = 0; i < 5; i++) {
        wait(&status);
        printf("Max Number: %d\n", WEXITSTATUS(status));
    }
    return 0;
}