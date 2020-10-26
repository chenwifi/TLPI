#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

static void handler(int sig){
	int savedErrno;
	sigset_t prev,tmp;
	struct sigaction sa;

	savedErrno = errno;

	printf("caught signal\n");
	if(signal(SIGTSTP,SIG_DFL) == SIG_ERR){
		fprintf(stderr,"signal\n");
		exit(EXIT_FAILURE);
	}

	raise(SIGTSTP);

	sigemptyset(&tmp);
	sigaddset(&tmp,SIGTSTP);
	sigprocmask(SIG_UNBLOCK,&tmp,&prev);

	sigprocmask(SIG_SETMASK,&prev,NULL);

	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(SIGTSTP,&sa,NULL);

	printf("exit handler\n");
	errno = savedErrno;
}

int main(int argc,char *argv[]){
	struct sigaction sa;
	sigset_t mask;

	sigprocmask(SIG_BLOCK,NULL,&mask);
	if(sigismember(&mask,SIGTSTP)){
		printf("sigtstp in mask\n");
	}

	sigaction(SIGTSTP,NULL,&sa);
	if(sa.sa_handler != SIG_IGN){
		sa.sa_handler = handler;
		sa.sa_flags = SA_RESTART;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGTSTP,&sa,NULL);
	}

	while(1){
		pause();
		printf("from main\n");
		sigprocmask(SIG_BLOCK,NULL,&mask);
		if(sigismember(&mask,SIGTSTP)){
			printf("sigtstp in mask\n");
		}
	}

	return 0;
}
