#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>

static volatile sig_atomic_t gotAlarm = 0;

static void handler(int sig){
	gotAlarm = 1;
}

static void displayTimes(const char *msg,int include){
	static struct timeval start;
	static int callNum = 0;
	struct timeval curr;
	struct itimerval itv;

	if(callNum == 0){
		gettimeofday(&start,NULL);
	}

	if(callNum % 20 == 0){
		printf("	Elapsed	  Value  Interval\n");
	}
	gettimeofday(&curr,NULL);
	printf("%-7s %6.2f",msg,curr.tv_sec - start.tv_sec + (curr.tv_usec - start.tv_usec) / 1000000.0);

	if(include){
		getitimer(ITIMER_REAL,&itv);
		printf("  %6.2f  %6.2f",itv.it_value.tv_sec + itv.it_value.tv_usec / 1000000.0,itv.it_interval.tv_sec + itv.it_interval.tv_usec / 1000000.0);
	}

	printf("\n");
	callNum++;
}

int main(int argc,char *argv[]){
	int maxSigs;
	int sigCnt;
	struct sigaction sa;
	struct itimerval itv;
	clock_t prevClock;

	if(argc > 1 && strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s [secs [usecs [int-secs [int-usecs]]]]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(SIGALRM,&sa,NULL);

	itv.it_value.tv_sec = (argc > 1) ? atoi(argv[1]) : 2;
	itv.it_value.tv_usec = (argc > 2) ? atoi(argv[2]) : 0;
	itv.it_interval.tv_sec = (argc > 3) ? atoi(argv[3]) : 0;
	itv.it_interval.tv_usec = (argc > 4) ? atoi(argv[4]) : 0;

	maxSigs = (itv.it_interval.tv_sec == 0 && itv.it_interval.tv_usec == 0) ? 1 : 3;

	displayTimes("START: ",0);
	setitimer(ITIMER_REAL,&itv,NULL);

	sigCnt = 0;
	while(1){
		prevClock = clock();
		while(((clock() - prevClock) * 10 / CLOCKS_PER_SEC) < 5){
			if(gotAlarm){
				gotAlarm = 0;
				displayTimes("ALARM: ",1);

				sigCnt++;
				if(sigCnt >= maxSigs){
					printf("That is all folks\n");
					exit(EXIT_SUCCESS);
				}
			}

		}
		displayTimes("MAIN: ",1);
	}

	return 0;
}
