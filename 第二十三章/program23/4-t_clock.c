#define _POSIX_C_SOURCE 199309
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
/*
#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
*/
/*
#define _POSIX_C_SOURCE 199309
#include <stdio.h>
#include <time.h>
*/

static volatile sig_atomic_t got = 0;

static void handler(int sig){
	got = 1;
}

int main(int argc,char *argv[]){
	/*
	struct timespec a,b;

	clock_getres(CLOCK_REALTIME,&a);
	printf("%2ld -- %09ld\n",a.tv_sec,a.tv_nsec);
	clock_gettime(CLOCK_REALTIME,&b);
	printf("%2ld -- %09ld\n",b.tv_sec,b.tv_nsec);
	clock_gettime(CLOCK_MONOTONIC,&b);
	printf("%2ld -- %09ld\n",b.tv_sec,b.tv_nsec);

	*/

	/*
	clockid_t clockid;
	struct timespec ts;
	struct sigaction sa;

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(SIGINT,&sa,NULL);

	//sleep(10);
	while(1){
		if(got == 1){
			break;
		}
	}

	printf("stop loop\n");

	clock_getcpuclockid(1,&clockid);
	clock_gettime(clockid,&ts);


	printf("CPU-time clock for PID %d is %ld.%09ld seconds\n",1,(long)ts.tv_sec,(long)ts.tv_nsec);
	*/

	struct sigaction sa;
	struct itimerval itv,prev;
	struct timespec req;

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(SIGALRM,&sa,NULL);

	itv.it_value.tv_sec = 4;
	itv.it_value.tv_usec = 0;
	itv.it_interval.tv_sec = 3;
	itv.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL,&itv,NULL);
	req.tv_sec = 2;
	req.tv_nsec = 0;
	nanosleep(&req,NULL);
	itv.it_value.tv_sec = 5;
	itv.it_value.tv_usec = 0;
	itv.it_interval.tv_sec = 6;
	itv.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL,&itv,&prev);

	printf("%d-%d-%d-%d\n",prev.it_value.tv_sec,prev.it_value.tv_usec,prev.it_interval.tv_sec,prev.it_interval.tv_usec);

	return 0;
}
