#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "4-signal_functions.h"

static int sigCnt[NSIG];
static volatile sig_atomic_t gotSigint = 0;

static void handler(int sig){
	//if(sig == SIGINT){
	if(sig == 2){
		printf("testing ok\n");
		gotSigint = 1;
	}else{
		sigCnt[sig]++;
	}
}

int main(int argc,char *argv[]){
	int i,sec;
	sigset_t blockSet,prevMask,pending,emptyMask;

	printf("%s : PID is %ld\n",argv[0],(long)getpid());
	printf("this is a line\n");

	for(i = 1;i < NSIG;i++){
		signal(i,handler);
	}

	if(argc > 1){
		sec = atoi(argv[1]);
		sigfillset(&blockSet);
		sigprocmask(SIG_BLOCK,&blockSet,&prevMask);

		sleep(sec);

		sigpending(&pending);
		fprintf(stderr,"%s: pending signals are: \n",argv[0]);
		printSigset(stderr,"\t\t",&pending);

		sigemptyset(&emptyMask);
		sigprocmask(SIG_BLOCK,&emptyMask,NULL);
	}

	//sleep(15);
	while(!gotSigint){
		continue;
	}
	/*
	while(!gotSigint){
		printf("%d\n",gotSigint);
		sleep(1);
		//pause();
	}
	*/

	for(i = 0;i < NSIG;i++){
		if(sigCnt[i] != 0){
			fprintf(stderr,"%s: signal %d caught %d times\n",argv[0],i,sigCnt[i]);
		}
	}

	return 0;
}
