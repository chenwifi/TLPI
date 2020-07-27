#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void sigHandler(int sig){
	printf("Ouch!\n");
}

int main(){

	if(signal(SIGINT,sigHandler) == SIG_ERR){
		fprintf(stderr,"signal");
		exit(EXIT_FAILURE);
	}

	while(1){
		printf("slowly loop\n");
		sleep(3);
	}

	return 0;
}
