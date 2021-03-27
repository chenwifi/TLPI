#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "1-pty_master_open.h"
#include "2-pty_fork.h"
#include "tty_functions.h"

#define MAX_SNAME 1000
#define BUF_SIZE 256

struct termios ttyOrig;

static void ttyReset(void){
	if(tcsetattr(STDIN_FILENO,TCSANOW,&ttyOrig) == -1){
		fprintf(stderr,"tcsetattr\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc,char *argv[]){
	struct winsize ws;
	int mfd,scriptFd;
	pid_t childPid;
	char slaveName[MAX_SNAME];
	char *shell;
	fd_set readsets;
	char buf[BUF_SIZE];
	ssize_t numRead;

	if(tcgetattr(STDIN_FILENO,&ttyOrig) == -1){
		fprintf(stderr,"tcgetattr\n");
		exit(EXIT_FAILURE);
	}

	if(ioctl(STDIN_FILENO,TIOCGWINSZ,&ws) == -1){
		fprintf(stderr,"ioctl\n");
		exit(EXIT_FAILURE);
	}

	childPid = ptyFork(&mfd,slaveName,MAX_SNAME,&ttyOrig,&ws);
	if(childPid == -1){
		fprintf(stderr,"ptyFork\n");
		exit(EXIT_FAILURE);
	}

	if(childPid == 0){
		shell = getenv("SHELL");
		if(shell == NULL || *shell == '\0'){
			shell = "/bin/sh";
		}

		execlp(shell,shell,(char *)NULL);
		exit(EXIT_FAILURE);
	}

	scriptFd = open((argc > 1) ? argv[1] : "typescript",O_WRONLY | O_CREAT | O_TRUNC,0666);
	if(scriptFd == -1){
		fprintf(stderr,"open\n");
		exit(EXIT_FAILURE);
	}

	ttySetRaw(STDIN_FILENO,&ttyOrig);
	if(atexit(ttyReset) != 0){
		fprintf(stderr,"atexit\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		FD_ZERO(&readsets);
		FD_SET(mfd,&readsets);
		FD_SET(STDIN_FILENO,&readsets);

		if(select(mfd + 1,&readsets,NULL,NULL,NULL) == -1){
			fprintf(stderr,"select\n");
			exit(EXIT_FAILURE);
		}

		if(FD_ISSET(mfd,&readsets)){
			numRead = read(mfd,buf,BUF_SIZE);
			if(numRead <= 0){
				fprintf(stderr,"read-1\n");
				exit(EXIT_FAILURE);
			}

			if(write(scriptFd,buf,numRead) != numRead){
				fprintf(stderr,"write\n");
				exit(EXIT_FAILURE);
			}
			if(write(STDOUT_FILENO,buf,numRead) != numRead){
				fprintf(stderr,"write\n");
				exit(EXIT_FAILURE);
			}
		}

		if(FD_ISSET(STDIN_FILENO,&readsets)){
			numRead = read(STDIN_FILENO,buf,BUF_SIZE);
			if(numRead <= 0){
				fprintf(stderr,"read-2\n");
				exit(EXIT_FAILURE);
			}

			if(write(mfd,buf,numRead) != numRead){
				fprintf(stderr,"write\n");
				exit(EXIT_FAILURE);
			}
		}
	}

	return 0;
}
