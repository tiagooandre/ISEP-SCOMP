#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int spawn_childs(int n) {
    int i;
    pid_t p;

    for (i = 0; i < n; i++) {
        p = fork();
        if (p < 0) {
            perror("Fork failed.\n");
            exit(1);
        }
        
        if (p == 0) {
            return i + 1;
        }
    }
    return 0;
}

int main() {
    pid_t p;
    int i, n, status;

    n = spawn_childs(6);

    if (n > 0) {
        exit(n * 2);
    }

    for (i = 0; i < 6; i++) {
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Valor de saída: %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}