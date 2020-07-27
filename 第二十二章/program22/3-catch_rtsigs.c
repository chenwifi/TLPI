#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

static volatile int handlerSleepTime;
static volatile int sigCnt = 0;
static volatile int allDone = 0;

static void siginfoHandler(int sig,siginfo_t *si,void *ucontext){
	if(sig == SIGINT || sig == SIGTERM){
		allDone = 1;
		printf("catch %d signals\n",sigCnt);
		return ;
	}

	sigCnt++;

	printf("	si_signo=%d,si_code=%d (%s)",si->si_signo,si->si_code,(si->si_code == SI_USER) ? "SI_USER" : (si->si_code == SI_QUEUE) ? "SI_QUEUE" : "other");
	printf("si_value=%d",si->si_value.sival_int);
	printf("si_pid=%ld,uid=%ld\n",(long)si->si_pid,(long)si->si_uid);

	sleep(handlerSleepTime);
}

int main(int argc,char *argv[]){
	struct sigaction sa;
	int i;
	sigset_t blockMask,prevMask;

	if(argc > 1 && strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s [block_time[handler_sleep_time]]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	printf("%s: PID is %ld\n",argv[0],(long)getpid());

	handlerSleepTime = (argc > 2) ? atoi(argv[2]) : 1;

	sa.sa_sigaction = siginfoHandler;
	sa.sa_flags = SA_SIGINFO;
	sigfillset(&sa.sa_mask);
	for(i = 1; i < NSIG;i++){
		if(i != SIGTSTP && i != SIGQUIT){
			sigaction(i,&sa,NULL);
		}
	}

	if(argc > 1){
		sigfillset(&blockMask);
		sigdelset(&blockMask,SIGINT);
		sigdelset(&blockMask,SIGTERM);

		sigprocmask(SIG_SETMASK,&blockMask,&prevMask);
		printf("%s: signals block sleep for %s\n",argv[0],argv[1]);
		sleep(atoi(argv[1]));
		sigprocmask(SIG_SETMASK,&prevMask,NULL);
	}

	while(!allDone){
		pause();
	}

	return 0;
}
