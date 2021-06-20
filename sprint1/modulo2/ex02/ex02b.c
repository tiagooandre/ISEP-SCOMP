#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    struct st {
        int num_st;
        char str_st[100];
    };

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

        struct st st_father;
        st_father.num_st = num;
        strcpy(st_father.str_st, str);
        
        write(fd[1], &st_father, sizeof(st_father));
        close(fd[1]);
    }

    if (p == 0) {
        close(fd[1]); //fecha escrita uma vez que vai ler
        struct st st_child;
        read(fd[0], &st_child, sizeof(st_child));
        printf("Child process: \n");
        printf("Number: %d \n", st_child.num_st);
        printf("String: %s \n", st_child.str_st);
        close(fd[0]);
    }
}