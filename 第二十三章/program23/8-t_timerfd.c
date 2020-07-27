#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timerfd.h>

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

int main(int argc,char *argv[]){
	struct itimerspec ts;
	uint64_t numExp,totalExp;
	int maxExp,fd,secs,nanosecs;
	struct timespec start,now;
	ssize_t s;

	itimerspecFromStr(argv[1],&ts);
	maxExp = (argc > 2) ? atoi(argv[2]) : 1;

	fd = timerfd_create(CLOCK_REALTIME,0);
	timerfd_settime(fd,0,&ts,NULL);

	clock_gettime(CLOCK_MONOTONIC,&start);

	for(totalExp = 0; totalExp < maxExp; ){
		s = read(fd,&numExp,sizeof(uint64_t));
		if(s != sizeof(uint64_t)){
			exit(EXIT_FAILURE);
		}

		totalExp += numExp;

		clock_gettime(CLOCK_MONOTONIC,&now);
		secs = now.tv_sec - start.tv_sec;
		nanosecs = now.tv_nsec - start.tv_nsec;
		if(nanosecs < 0){
			secs--;
			nanosecs += 1000000000;
		}

		printf("%d.%03d: expiration read:%llu;total=%llu\n",secs,(nanosecs + 500000) / 1000000,(unsigned long long)numExp,(unsigned long long)totalExp);
	}

	return 0;
}
