#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int cnt = 0;

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

static void threadFunc(union sigval sv){
	timer_t *tidptr;

	tidptr = sv.sival_ptr;

	pthread_mutex_lock(&mtx);
	printf("timer ID : %ld\n",(long)*tidptr);
	printf("timer_getoverrun() = %d\n",timer_getoverrun(*tidptr));
	cnt += 1 + timer_getoverrun(*tidptr);
	pthread_mutex_unlock(&mtx);
	pthread_cond_signal(&cond);
}

int main(int argc,char *argv[]){
	struct sigevent sev;
	int i;
	struct itimerspec ts;
	timer_t *tidlist;

	tidlist = calloc(argc - 1,sizeof(timer_t));

	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = threadFunc;
	sev.sigev_notify_attributes = NULL;

	for(i = 0; i < argc - 1; i++){
		itimerspecFromStr(argv[i + 1],&ts);
		sev.sigev_value.sival_ptr = &tidlist[i];
		timer_create(CLOCK_REALTIME,&sev,&tidlist[i]);
		printf("Timer ID: %ld\n",tidlist[i]);
		timer_settime(tidlist[i],0,&ts,NULL);
	}

	pthread_mutex_lock(&mtx);
	while(1){
		pthread_cond_wait(&cond,&mtx);
		printf("main: cnt = %d\n",cnt);
	}

	return 0;
}
