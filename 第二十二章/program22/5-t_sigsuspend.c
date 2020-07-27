//#define _POSIX_C_SOURCE 199309
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <signal.h>
#include "signal_functions.h"

static volatile sig_atomic_t gotSigquit = 0;

static void handler(int sig){
	//printf("caught signal %d (%s)\n",sig,strsignal(sig));
	printf("caught signal %d (s)\n",sig);

	if(sig == SIGQUIT){
		gotSigquit = 1;
	}
}

int main(int argc,char *argv[]){
	struct sigaction sa;
	sigset_t blockSet,prevSet;
	int loop;
	time_t t;

	printSigMask(stdout,"Initial signal mask:\n");

	sigemptyset(&blockSet);
	sigaddset(&blockSet,SIGINT);
	sigaddset(&blockSet,SIGQUIT);
	sigprocmask(SIG_BLOCK,&blockSet,&prevSet);

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(SIGINT,&sa,NULL);
	sigaction(SIGQUIT,&sa,NULL);

	for(loop = 1;gotSigquit != 1;loop++){
		printf("====loop %d\n",loop);
		
		printSigMask(stdout,"starting critical section,signal mask:\n ");

		for(t = time(NULL);time(NULL) < t + 4;){
			continue;
		}

		printPendingSigs(stdout,"before sigsuspend() pending signals:\n");

		if(sigsuspend(&prevSet) == -1 && errno != EINTR ){
			fprintf(stderr,"sigsuspend\n");
			exit(EXIT_FAILURE);
		}
	}

	sigprocmask(SIG_SETMASK,&prevSet,NULL);
	printSigMask(stdout,"exit loop signal mask:\n");

	return 0;
}
