#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>

void displayProcessTimes(char *msg){
	static long clockTicks = 0;
	clock_t clockTime;
	struct tms t;

	if(msg != NULL){
		printf("%s",msg);
	}

	if(clockTicks == 0){
		clockTicks = sysconf(_SC_CLK_TCK);
		if(clockTicks == -1){
			exit(EXIT_FAILURE);
		}
	}

	clockTime = clock();
	if(clockTime == -1){
		exit(EXIT_FAILURE);
	}
	printf("    clock returns: %ld clocks_per_sec (%.2f secs)\n",(long)clockTime,(double)clockTime/CLOCKS_PER_SEC);

	if(times(&t) == -1){
		exit(EXIT_FAILURE);
	}
	printf("    times() yields: user CPU=%.2f;system CPU=%.2f\n",(double)t.tms_utime/clockTicks,(double)t.tms_stime/clockTicks);
}

int main(int argc,char *argv[]){
	int j,numCalls;

	printf("CLOCKS_PER_SEC = %ld   sysconf(_SC_CLK_TCK)=%ld\n\n",CLOCKS_PER_SEC,sysconf(_SC_CLK_TCK));

	displayProcessTimes("At program start:\n");
	numCalls = argc > 1 ? atol(argv[1]) : 100000000;
	for(j = 0;j < numCalls;j++){
		getppid();
	}
	displayProcessTimes("After getppid() loop:\n");

	return 0;
}
