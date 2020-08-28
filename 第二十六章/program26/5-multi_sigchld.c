#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "2-print_wait_status.h"

static volatile int numLiveChildren = 0;

static void handler(int sig){
	int saveError,status;
	pid_t childPid;

	saveError = errno;

	sleep(4);

	while((childPid = waitpid(-1,&status,WNOHANG)) > 0){
		numLiveChildren--;
		printf("handler reap child PID=%ld \n",(long)childPid);
		printWaitStatus(NULL,status);
	}

	//you should know about this !!!this will bug!!!
	if(childPid == -1 /*&& errno != ECHILD*/){
		fprintf(stderr,"waitpid");
		exit(EXIT_FAILURE);
	}
	sleep(5);
	printf("handler returning \n");

	errno = saveError;
}

int main(int argc,char *argv[]){
	int i,sigCnt;
	sigset_t empty,block;
	struct sigaction sa;

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"usage: %s sleepsec ...\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(SIGCHLD,&sa,NULL);

	sigemptyset(&block);
	sigaddset(&block,SIGCHLD);
	sigprocmask(SIG_SETMASK,&block,NULL);

	numLiveChildren = argc -1;

	for(i = 1; i < argc; i++){
		switch(fork()){
			case -1:
				fprintf(stdout,"fork\n");
				exit(EXIT_SUCCESS);
				break;
			case 0:
				sleep(atoi(argv[i]));
				printf("child PID=%ld exiting\n",(long)getpid());
				_exit(EXIT_SUCCESS);
				break;
			default:
				break;
		}
	}

	sigCnt = 0;
	sigemptyset(&empty);
	while(numLiveChildren){
		if(sigsuspend(&empty) == -1 && errno != EINTR)
			fprintf(stdout,"sigsuspend\n");
		sigCnt++;
	}

	printf("All %d children are dead;SIGCHLD was caugth %d time\n",argc - 1,sigCnt);

	return 0;
}
