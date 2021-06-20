#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

#define ARRAY_SIZE 1000

int main() {
    int numbers[ARRAY_SIZE], n, i, j, max, status, result[ARRAY_SIZE];
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
            exit(-1);
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
    
    max = 0;
    for (i = 0; i < 5; i++) {
        wait(&status);
        if (max < WEXITSTATUS(status)) {
            max = WEXITSTATUS(status);
        }
    }

    p = fork();

    if (p < 0) {
        perror("Fork failed.\n");
        exit(-1);
    }

    if (p == 0) {
        for (i = 0; i < ARRAY_SIZE/2; i++) {
            result[i] = ((int) numbers[i]/max)*100;
            printf("Result[%d]: %d\n", i, result[i]);
        }
        exit(0);
    }

    return 0;
}