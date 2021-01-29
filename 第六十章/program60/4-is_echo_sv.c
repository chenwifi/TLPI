#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include "inet_sockets.h"

#define SERVICE "echo"
#define BUF_SIZE 4096

static void handler(int sig){
	int savedErrno;

	savedErrno = errno;
	while(waitpid(-1,NULL,WNOHANG) > 0){
		continue;
	}

	errno = savedErrno;
}

static void handleRequest(int cfd){
	char buf[BUF_SIZE];
	ssize_t numRead;

	while((numRead = read(cfd,buf,BUF_SIZE)) > 0){
		if(write(cfd,buf,numRead) != numRead){
			fprintf(stderr,"write\n");
			exit(EXIT_FAILURE);
		}
	}

	if(numRead == -1){
		fprintf(stderr,"read\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc,char *argv[]){
	int sfd,cfd;
	struct sigaction sa;

	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	if(sigaction(SIGCHLD,&sa,NULL) == -1){
		fprintf(stderr,"sigaction\n");
		exit(EXIT_FAILURE);
	}

	sfd = inetListen(SERVICE,10,NULL);
	if(sfd == -1){
		fprintf(stderr,"inetListen\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		cfd = accept(sfd,NULL,NULL);
		if(cfd == -1){
			fprintf(stderr,"accept\n");
			exit(EXIT_FAILURE);
		}

		switch(fork()){
			case -1:
				close(cfd);
				fprintf(stderr,"fork\n");
				break;
			case 0:
				close(sfd);
				handleRequest(cfd);
				_exit(EXIT_SUCCESS);
				break;
			default:
				close(cfd);
				break;
		}
	}

	return 0;
}
