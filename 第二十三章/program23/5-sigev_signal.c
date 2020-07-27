#define _POSIX_C_SOURCE 199309
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#define TIMER_SIG SIGRTMAX


void itimerspecFromStr(char *str,struct itimerspec *tsp){
	char *cptr,*sptr;

	cptr = strchr(str,':');
	if(cptr != NULL){
		*cptr = '\0';
	}
	sptr = strchr(str,'/');
	if(sptr != NULL){
		*sptr = '\0';
	}

	tsp->it_value.tv_sec = atoi(str);
	tsp->it_value.tv_nsec = (sptr != NULL) ? atoi(str + 1) : 0;

	if(cptr == NULL){
		tsp->it_interval.tv_sec = 0;
		tsp->it_interval.tv_nsec = 0;
	}else{
		sptr = strchr(cptr + 1,'/');
		if(sptr != NULL){
			*sptr = '\0';
		}
		tsp->it_interval.tv_sec = atoi(cptr + 1);
		tsp->it_interval.tv_nsec = (sptr != NULL) ?  atoi(sptr + 1) : 0;
	}
}

static void handler(int sig,siginfo_t *info,void *uc){
	timer_t *tidptr;

	tidptr = info->si_value.sival_ptr;
	printf("got a signal: %d\n",sig);
	printf("*sival_ptr = %ld\n",(long)*tidptr);
	printf("timer_getoverrun=%ld\n",timer_getoverrun(*tidptr));
}

int main(int argc,char *argv[]){
	timer_t *tidlist;
	struct sigaction sa;
	struct itimerspec ts;
	struct sigevent sev;
	int i;

	tidlist = calloc(argc -1,sizeof(timer_t));

	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	sigaction(TIMER_SIG,&sa,NULL);

	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = TIMER_SIG;

	for(i = 0; i < argc - 1; i++){
		itimerspecFromStr(argv[i + 1],&ts);
		sev.sigev_value.sival_ptr = &tidlist;
		timer_create(CLOCK_REALTIME,&sev,&tidlist[i]);
		printf("Timer ID: %ld \n",(long)tidlist[i]);
		timer_settime(tidlist[i],0,&ts,NULL);
	}

	while(1){
		pause();
	}

	return 0;
}
