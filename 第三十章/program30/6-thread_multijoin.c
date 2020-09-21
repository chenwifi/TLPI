#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int totThreads = 0;
static int numLive = 0;

static int numUnjoined = 0;

enum tstate{
	TS_ALIVE,
	TS_TERMINATED,
	TS_JOINED,
};

static struct thread{
	enum tstate state;
	pthread_t id;
	int sleepTime;
}*threadp;

static void *threadFunc(void *arg){
	struct thread *tmp;
	int sec;

	tmp = (struct thread *)arg;
	sleep(tmp->sleepTime);

	pthread_mutex_lock(&mtx);
	numUnjoined++;
	tmp->state = TS_TERMINATED;
	pthread_mutex_unlock(&mtx);

	pthread_cond_signal(&cond);

	return ;
}

int main(int argc,char *argv[]){
	int i,s;

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s <sleeptime>...",argv[0]);
		exit(0);
	}

	threadp = (struct thread *)calloc(argc - 1,sizeof(struct thread));
	if(threadp == NULL){
		fprintf(stderr,"calloc\n");
		exit(0);
	}

	for(i = 0; i < argc - 1; i++){
		threadp[i].state = TS_ALIVE;
		threadp[i].sleepTime = atoi(argv[i + 1]);
		s = pthread_create(&threadp[i].id,NULL,threadFunc,threadp + i);
		if(s > 0){
			fprintf(stderr,"pthread_create\n");
			exit(0);
		}
	}

	totThreads = argc - 1;
	numLive = totThreads;

	while(numLive > 0){
		pthread_mutex_lock(&mtx);

		while(numUnjoined = 0){
			pthread_cond_wait(&cond,&mtx);
		}

		for(i = 0; i < totThreads; i++){
			if(threadp[i].state == TS_TERMINATED){
				pthread_join(threadp[i].id,NULL);
				threadp[i].state = TS_JOINED;
				printf("thread %d joined\n",i);
				numLive--;
				numUnjoined--;
			}
		}

		pthread_mutex_unlock(&mtx);
	}

	return 0;
}
