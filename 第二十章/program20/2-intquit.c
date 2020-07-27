#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void sigHandler(int sig){
	static int count = 0;


	if(sig == SIGINT){
		count++;
		printf("Caught SIGINT (%d)\n",count);
		sleep(5);
		return;
	}

	printf("Caught SIGQUIT\n");
	
	sleep(5);

	return;
}

int main(){

	if(signal(SIGINT,sigHandler) == SIG_ERR){
		fprintf(stderr,"signal");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGQUIT,sigHandler) == SIG_ERR){
		fprintf(stderr,"signal");
		exit(EXIT_FAILURE);
	}

	while(1){
		pause();
	}

	return 0;
}
