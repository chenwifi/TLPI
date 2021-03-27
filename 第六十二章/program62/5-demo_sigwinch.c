#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <signal.h>

void handler(int sig){
}

int main(int argc,char *argv[]){
	struct sigaction sa;
	struct winsize ws;

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(SIGWINCH,&sa,NULL);

	while(1){
		pause();

		if(ioctl(STDIN_FILENO,TIOCGWINSZ,&ws) == -1){
			fprintf(stderr,"ioctl\n");
			exit(EXIT_FAILURE);
		}

		printf("Caught SIGWINCH,new windows size: %d rows * %d columns\n",ws.ws_row,ws.ws_col);
	}

	return 0;
}
