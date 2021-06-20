#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

const char* filename = "text.txt";
FILE *file;
char *line;
size_t size;

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

    if (p > 0) {
        close(fd[0]);
        file = fopen(filename, "r");
        if (file == NULL) {
            exit(-1);
        }
        
        while ((getline(&line, &size, file)) != -1) {
            write(fd[1], line, strlen(line));
        }
        fclose(file);
        close(fd[1]);
        
        wait(NULL);
        printf("\n\n---> The child process finished. <---\n");
    } else {
        close(fd[1]);
        
        char file_str[100];
		while(read(fd[0], file_str, sizeof(file_str))){
			printf("%s ", file_str);
		}
        
        close(fd[0]);
    }
}