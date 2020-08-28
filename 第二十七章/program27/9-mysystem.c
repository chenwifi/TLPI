#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "print_wait_status.h"

#define LEN 200

int mysystem(const char *command){
	sigset_t blockMask,originMask;
	struct sigaction saIgnore,saOrigQuit,saOrigInt,saDefault; 
	pid_t childPid;
	int status,savedErrno;

	if(command == NULL){
		return mysystem(":") == 0;
	}

	sigemptyset(&blockMask);
	sigaddset(&blockMask,SIGCHLD);
	sigprocmask(SIG_BLOCK,&blockMask,&originMask);

	saIgnore.sa_flags = 0;
	saIgnore.sa_handler = SIG_IGN;
	sigemptyset(&saIgnore.sa_mask);
	sigaction(SIGINT,&saIgnore,&saOrigInt);
	sigaction(SIGQUIT,&saIgnore,&saOrigQuit);

	switch(childPid = fork()){
		case -1:
			status = -1;
			break;
		case 0:
			saDefault.sa_flags = 0;
			saDefault.sa_handler = SIG_DFL;
			sigemptyset(&saDefault.sa_mask);

			if(saOrigInt.sa_handler != SIG_IGN){
				sigaction(SIGINT,&saDefault,NULL);
			}

			if(saOrigQuit.sa_handler != SIG_IGN){
				sigaction(SIGQUIT,&saDefault,NULL);
			}

			sigprocmask(SIG_SETMASK,&originMask,NULL);
			execl("/bin/sh","sh","-c",command,(char *)NULL);
			_exit(127);

			break;
		default:
			while(waitpid(childPid,&status,NULL) == -1){
				if(errno != EINTR){
					status = -1;
					break;
				}
			}
			break;
	}

	savedErrno = errno;
	sigprocmask(SIG_SETMASK,&originMask,NULL);
	sigaction(SIGINT,&saOrigInt,NULL);
	sigaction(SIGQUIT,&saOrigQuit,NULL);
	errno = savedErrno;

	return status;
}

int main(int argc,char *argv[]){
	char buf[LEN];
	int status;

	while(1){
		printf("Command: ");
		fflush(stdout);
		
		fgets(buf,LEN,stdin);
		status = mysystem(buf);
		printf("system return: status=0x%04x(%d,%d)\n",status,status >> 8,status & 0x0f);
		
		if(status == -1){
			fprintf(stdout,"system error\n");
			exit(EXIT_SUCCESS);
		}else{
			if(WIFEXITED(status) && WEXITSTATUS(status) == 127){
				printf("probably could not invoke shell\n");
			}else{
				printWaitStatus(NULL,status);
			}
		}
	}

	return 0;
}
