#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig){
	printf("got signal\n");
}

int main(int argc,char *argv[]){
	struct sigaction sa;

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(SIGINT,&sa,NULL);

	switch(fork()){
		case -1:
			break;
		case 0:
			break;
		default:
			break;
	}

	pause();

	return 0;
}
