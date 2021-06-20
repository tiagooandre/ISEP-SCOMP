#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

const int still = 1, finish = 0;

int main () {
    time_t t;
	srand((unsigned) time(&t));

    int fd1[2]; //Parent writes, child reads
    int fd2[2]; //Opposite

    if (pipe(fd1) == -1) {
        perror("Pipe 1 failed.\n");
        return 1;
    }

    if (pipe(fd2) == -1) {
        perror("Pipe 2 failed.\n");
        return 1;
    }

    pid_t p;
    int credit_c = 0, bet_c = 0;

    p = fork();

    if (p < 0) {
        perror("Fork failed.\n");
        return 1;
    }

    
    if (p > 0) {
        //srand(time(NULL));

        close(fd2[1]);
        close(fd1[0]);
        credit_c = 20;
        while (credit_c > 0) {
            int r = rand() % 5 + 1; //Random number between 1 and 5
            printf("Random number: %d\n", r);

            write(fd1[1], &still, sizeof(still));
            read(fd2[0], &bet_c, sizeof(bet_c));

            if (bet_c == r) {
                printf("Child bet %d was right!\n", bet_c);
                credit_c += 10;
            } else {
                printf("Child bet %d was not right...\n", bet_c);
                credit_c -= 5;
            }
            printf("Remaining Credit: %d\n\n\n", credit_c);

            write(fd1[1], &credit_c, sizeof(credit_c));
        }
        write(fd1[1], &finish, sizeof(finish));
        close(fd2[0]);
        close(fd1[1]);
    }

    int notification = 1;

    if (p == 0) {
        srand(time(NULL) * getpid());

        close(fd2[0]);
        close(fd1[1]);

        while (notification) {
            read(fd1[0], &notification, sizeof(notification));
            int r = rand() % 5 + 1; //Random number between 1 and 5
            write(fd2[1], &r, sizeof(r));
        }
        close(fd2[1]);
        close(fd1[0]);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_SUCCESS);
}