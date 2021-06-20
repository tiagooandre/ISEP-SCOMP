#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int sigaction (int sig, const struct sigaction * act, struct sigaction * oact);
static int flag;

void handle_CHLD(int signo, siginfo_t *sinfo, void *context) {
    flag = 0;
}

int main () {
    char str_in[100];
    int count = 0;
    struct sigaction act;

    /*Limpar a variável act*/
    memset(&act, 0, sizeof(struct sigaction));

    /*Para bloquear sinais*/
    sigfillset(&act.sa_mask); 

    act.sa_sigaction = handle_CHLD;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGCHLD, &act, NULL);

    int p;
    p = fork();
    flag = -1;

    if (p != 0) {
        sleep(10);
        if (flag == 0) {
            sleep(10);
            printf("Successful execution!\n");
        } else {
            kill(p, SIGINT);
            char str_m[100] = "To slow, that is why the program will end!\n";
            write(STDOUT_FILENO, str_m, strlen(str_m));
            
        }
    } else {
        char str[100] = "Write the string: \n";
        write(STDOUT_FILENO, str, strlen(str));
        fgets(str_in, 100, stdin);

        for (int i = 0; i < strlen(str_in); i++) {
            if (str_in[i] != 10 && str_in[i] != 32) {
                count++;
            }
        }
        printf("The number of characters: %d\n", count);
    }
}