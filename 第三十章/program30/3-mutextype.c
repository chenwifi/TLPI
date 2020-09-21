#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int glob = 0;

//static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; 

static void *threadFunc(void *arg){
	int loop,i,s;
	int tmp;
	pthread_mutex_t mtx;
	pthread_mutexattr_t attr;

	loop = (*(int *)arg);

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mtx,&attr);

	pthread_mutexattr_destroy(&attr);
	s = pthread_mutex_lock(&mtx);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}
	s = pthread_mutex_lock(&mtx);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}
	
	for(i = 0; i < loop; i++){
		tmp = glob;
		tmp++;
		glob = tmp;
	}

	s = pthread_mutex_unlock(&mtx);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}

}

int main(int argc,char *argv[]){
	pthread_t t1,t2;
	int loop,s;

	loop = (argc > 1) ? atoi(argv[1]) : 10000000;

	s = pthread_create(&t1,NULL,threadFunc,&loop);
	if(s != 0){
		fprintf(stderr,"pthread_create\n");
		exit(0);
	}

	/*
	s = pthread_create(&t2,NULL,threadFunc,(void *)&loop);
	if(s != 0){
		fprintf(stderr,"pthread_create\n");
		exit(0);
	}
	*/

	s = pthread_join(t1,NULL);
	if(s != 0){
		fprintf(stderr,"pthread_join\n");
		exit(0);
	}
	
	/*
	s = pthread_join(t2,NULL);
	if(s != 0){
		fprintf(stderr,"pthread_join\n");
		exit(0);
	}
	*/

	printf("glob=%d\n",glob);

	return 0;
}
