#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
  int p = fork();
  int status1, status2;

  if (p < 0) {
    perror("Fork failed.\n");
    exit(-1);
  }

  if (p == 0) {
    sleep(1);
    exit(1);
  }

  p = fork();
  if (p < 0) {
    perror("Fork failed.\n");
    exit(-1);
  } 

  if (p == 0) {
    sleep(2);
    exit(2);
  }

  wait(&status1);
  if (WIFEXITED(status1)) {
    printf("%d\n", WEXITSTATUS(status1));
  }

  wait(&status2);
  if (WIFEXITED(status2)) {
    printf("%d\n", WEXITSTATUS(status2));
  }
  
  return 0;
}