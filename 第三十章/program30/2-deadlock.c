#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

static int glob = 0;

static pthread_mutex_t mtx1 = PTHREAD_MUTEX_INITIALIZER; 
static pthread_mutex_t mtx2 = PTHREAD_MUTEX_INITIALIZER; 

static void *threadFunc2(void *arg){
	int loop,i,s;
	int tmp;

	loop = (*(int *)arg);

	s = pthread_mutex_lock(&mtx2);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}

	sleep(5);

	s = pthread_mutex_lock(&mtx1);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}
	
	for(i = 0; i < loop; i++){
		tmp = glob;
		tmp++;
		glob = tmp;
	}

	s = pthread_mutex_unlock(&mtx2);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}
	s = pthread_mutex_unlock(&mtx1);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}

}

static void *threadFunc1(void *arg){
	int loop,i,s;
	int tmp;

	loop = (*(int *)arg);

	s = pthread_mutex_lock(&mtx1);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}

	sleep(5);

	s = pthread_mutex_lock(&mtx2);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}
	
	for(i = 0; i < loop; i++){
		tmp = glob;
		tmp++;
		glob = tmp;
	}

	s = pthread_mutex_unlock(&mtx1);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}
	s = pthread_mutex_unlock(&mtx2);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}

}

int main(int argc,char *argv[]){
	pthread_t t1,t2;
	int loop,s;

	loop = (argc > 1) ? atoi(argv[1]) : 10000000;

	s = pthread_create(&t1,NULL,threadFunc1,&loop);
	if(s != 0){
		fprintf(stderr,"pthread_create\n");
		exit(0);
	}

	s = pthread_create(&t2,NULL,threadFunc2,(void *)&loop);
	if(s != 0){
		fprintf(stderr,"pthread_create\n");
		exit(0);
	}

	s = pthread_join(t1,NULL);
	if(s != 0){
		fprintf(stderr,"pthread_join\n");
		exit(0);
	}
	
	s = pthread_join(t2,NULL);
	if(s != 0){
		fprintf(stderr,"pthread_join\n");
		exit(0);
	}

	printf("glob=%d\n",glob);

	return 0;
}
