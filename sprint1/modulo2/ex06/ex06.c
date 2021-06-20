#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

int main() {
    int vec1[1000], vec2[1000];
    time_t t;
	srand((unsigned) time(&t));

    for (int i = 0; i < 1000; i++) {
        vec1[i] = abs(rand()) % 100;
        vec2[i] = abs(rand()) % 100;
    }

    int fd[2];

    if (pipe(fd) == -1) {
        perror("Pipe failed.\n");
        return 1;
    }

    pid_t p;
    int status, tmp;

    for (int i = 0; i < 5; i++) {
        p = fork();
        
        if (p < 0) {
            perror("Fork failed.\n");
            return 1;
        }

        if (p == 0) {
            close(fd[0]);
            int start = i * 200;
            int finish = start + 200;
            for (int j = start; j < finish; j++) {
                tmp += vec1[j] + vec2[j];
            }

            write(fd[1], &tmp, sizeof(tmp));
            close(fd[1]);
            exit(0);
        }
    }

    close(fd[1]);

    int p_sum, sum = 0;

    for (int i = 0; i < 5; i++) {
        wait(&status);
        p_sum = 0;
        read(fd[0], &p_sum, sizeof(p_sum));
        printf("Partial sum - parent process: %d\n", p_sum);
        sum += p_sum;
    }

    close(fd[0]);

    for (int i = 0; i < 200; i++) {
        printf("Vec1: %d\n", vec1[i]);
    }

    for (int i = 0; i < 200; i++) {
        printf("Vec2: %d\n", vec2[i]);
    }

    printf("Total sum: %d\n", sum);
}