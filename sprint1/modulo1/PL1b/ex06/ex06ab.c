#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

int USR1_COUNTER = 0;

int sigaction (int sig, const struct sigaction * act, struct sigaction * oact);

void handle_USR1 (int signo, siginfo_t *sinfo, void *context) {
    USR1_COUNTER++;

    /*Forma não segura de obter output*/
    printf("SIGUSR1 signal captured, USR1_COUNTER = %d\n", USR1_COUNTER);
}

int main (int argc, char *argv[]) {
    struct sigaction act;

    /*Limpar a variável act*/
    memset(&act, 0, sizeof(struct sigaction));

    /*Para bloquear todos os sinais*/
    sigfillset(&act.sa_mask); 

    act.sa_sigaction = handle_USR1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    while (1) {
        printf("I'm working!");
        fflush(stdout);
        sleep(1);
    }
}