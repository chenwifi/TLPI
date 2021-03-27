#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "tty_functions.h"

static volatile sig_atomic_t gotSigio = 0;

static void handler(int sig){
	gotSigio = 1;
}

int main(int argc,char *argv[]){
	struct sigaction sa;
	struct termios ourTermios;
	int flags,done,i,cnt;
	char buf;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
	if(sigaction(SIGIO,&sa,NULL) == -1){
		fprintf(stderr,"sigaction\n");
		exit(EXIT_FAILURE);
	}

	if(fcntl(STDIN_FILENO,F_SETOWN,getpid()) == -1){
		fprintf(stderr,"fcntl\n");
		exit(EXIT_FAILURE);
	}

	if((flags = fcntl(STDIN_FILENO,F_GETFL)) == -1){
		fprintf(stderr,"fcntl\n");
		exit(EXIT_FAILURE);
	}
	if(fcntl(STDIN_FILENO,F_SETFL,flags | O_ASYNC | O_NONBLOCK) == -1){
		fprintf(stderr,"fcntl\n");
		exit(EXIT_FAILURE);
	}

	ttySetCbreak(STDIN_FILENO,&ourTermios);

	for(done = 0,cnt = 0; !done; cnt++){
		for(i = 0; i < 100000000; i++){
			continue;
		}
		if(gotSigio){

			while(read(STDIN_FILENO,&buf,1) > 0 && !done){
				printf("cnt = %d and read %c\n",cnt,buf);
				if(buf == '#'){
					done = 1;
				}
			}

			gotSigio = 0;
			/*
			if(read(STDIN_FILENO,&buf,1) != 1){
				fprintf(stderr,"fcntl\n");
				exit(EXIT_FAILURE);
			}else{
				printf("%c\n",buf);
				if(buf == '#'){
					done = 1;
				}
				gotSigio = 0;
			}
			*/
		}
	}

	if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&ourTermios) == -1){
		fprintf(stderr,"tcsetattr\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
