#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>

struct st {
    char sms[10];
    int num_round;
};

int main () {
    struct st st1;
    int fd[2];

    if (pipe(fd) == -1) {
        perror("Pipe failed.\n");
        return 1;
    }

    pid_t p;

    for (int i = 0; i < 10; i++) {
        p = fork();

        if (p < 0) {
            perror("Fork failed.\n");
            return 1;
        }

        if (p == 0) {
            close(fd[1]);
            if (read(fd[0], &st1, sizeof(st1)) != 0) {
                printf("Winner! Round number %d\n", st1.num_round);
                exit(st1.num_round);
            }
            close(fd[0]);
        }
    }

    for (int i = 0; i < 10; i++) {
        sleep(2);

        st1.num_round = i+1;
        strcpy(st1.sms, "Win");

        close(fd[0]);
        write(fd[1], &st1, sizeof(st1));
    }

    int status = 0;

    for (int i = 0; i < 10; i++) {
        pid_t pid = wait(&status);
        sleep(1); //Para o printf ser imprimido apenas depois da última round
        printf("Round %d with Child's PID %d!\n", WEXITSTATUS(status), pid);
    }
}