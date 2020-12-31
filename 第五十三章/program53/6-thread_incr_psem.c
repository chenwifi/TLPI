#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

static int glob = 0;
static sem_t sem;

static void *threadFunc(void *arg){
	int loops;
	int i,loc;

	loops = *((int *)arg);

	for(i = 0; i < loops; i++){
		if(sem_wait(&sem) == -1){
			fprintf(stderr,"sem_wait\n");
			exit(EXIT_FAILURE);
		}

		loc = glob;
		loc++;
		glob = loc;

		if(sem_post(&sem) == -1){
			fprintf(stderr,"sem_post\n");
			exit(EXIT_FAILURE);
		}
	}

	return NULL;
}

int main(int argc,char *argv[]){
	pthread_t t1,t2;
	int loops,s;

	loops = (argc > 1) ? atoi(argv[1]) : 10000000;

	if(sem_init(&sem,0,1) == -1){
		fprintf(stderr,"sem_init\n");
		exit(EXIT_FAILURE);
	}

	s = pthread_create(&t1,NULL,threadFunc,&loops);
	if(s != 0){
		fprintf(stderr,"pthread_create\n");
		exit(EXIT_FAILURE);
	}
	s = pthread_create(&t2,NULL,threadFunc,&loops);
	if(s != 0){
		fprintf(stderr,"pthread_create\n");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(t1,NULL);
	if(s != 0){
		fprintf(stderr,"pthread_join\n");
		exit(EXIT_FAILURE);
	}
	s = pthread_join(t2,NULL);
	if(s != 0){
		fprintf(stderr,"pthread_join\n");
		exit(EXIT_FAILURE);
	}

	printf("glob = %d\n",glob);

	return 0;
}
