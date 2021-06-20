#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int main() {
	pid_t p;
	int sig = 0;
	
	
	for(;;){
		
		scanf("%d", &p);
		scanf("%d", &sig);
		
		printf("PID: %d \n", p);
		printf("Signal Number: %d\n", sig);
	
		printf("I Love SCOMP!\n");
		sleep(1);
		kill(p, sig);
	}

	return 0;
}