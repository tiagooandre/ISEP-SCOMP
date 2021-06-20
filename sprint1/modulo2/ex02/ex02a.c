#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];

    if(pipe(fd) == -1) {
        perror("Pipe failed.\n");
        return 1;
    }

    pid_t p = fork();

    if (p < 0) {
        perror("Fork failed.\n");
        return 1;
    }

    int num;
    char str[100];

    if (p > 0) {
        close(fd[0]); //fecha leitura uma vez que vai escrever
        printf("Write a number: \n");
        scanf("%d", &num);
        printf("Write a string: \n");
        scanf("%s", str);
        write(fd[1], &num, sizeof(num)); 
        write(fd[1], &str, sizeof(str)+1);
        close(fd[1]);
    }

    if (p == 0) {
        close(fd[1]); //fecha escrita uma vez que vai ler
        read(fd[0], &num, sizeof(num)); 
        read(fd[0], &str, sizeof(str)+1);
        close(fd[0]);
        printf("Child process: \n");
        printf("Number: %d \n", num);
        printf("String: %s \n", str);
    }
}