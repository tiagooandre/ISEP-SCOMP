#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main() {
  int i, p;

  for (i = 0; i < 10; i++) {
    p = fork();
    if (p == 0) {
      int j;
      for (j = i * 100 + 1; j < (i + 1) * 100; j++) {
	    printf("%i\n", j);
      }
      exit(i);
    }
  }
  wait(0);
}