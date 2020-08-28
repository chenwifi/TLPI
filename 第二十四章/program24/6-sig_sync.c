#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define SYNC_SIG SIGUSR1

static void handler(int sig){
}

int main(int argc,char *argv[]){
	pid_t cpid;
	sigset_t blockSet,prevSet,pendSet;
	struct sigaction sa;

	setbuf(stdout,NULL);

	sigemptyset(&blockSet);
	sigaddset(&blockSet,SYNC_SIG);
	sigprocmask(SIG_BLOCK,&blockSet,&prevSet);

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(SYNC_SIG,&sa,NULL);

	switch(cpid = fork()){
		case -1:
			break;
		case 0:
			sigprocmask(SIG_SETMASK,&prevSet,NULL);
			printf("child first\n");
			kill(getppid(),SYNC_SIG);
			_exit(EXIT_FAILURE);
			break;
		default:
			sigemptyset(&pendSet);
			sigsuspend(&pendSet);
			sigprocmask(SIG_SETMASK,&prevSet,NULL);
			printf("parent last\n");
			break;
	}

	return 0;
}
