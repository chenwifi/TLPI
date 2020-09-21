#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *threadFunc(void *arg){
	int i;

	i = 0;
	while(1){
		i++;
		printf("loop %d\n",i);
		sleep(1);
	}
}

int main(int argc,char *argv[]){
	pthread_t t;
	int s;
	void *res;

	s = pthread_create(&t,NULL,threadFunc,NULL);
	if(s != 0){
		fprintf(stderr,"pthread_create\n");
		exit(EXIT_FAILURE);
	}

	sleep(3);

	s = pthread_cancel(t);
	if(s != 0){
		fprintf(stderr,"pthread_cancel\n");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(t,&res);
	if(s != 0){
		fprintf(stderr,"pthread_join\n");
		exit(EXIT_FAILURE);
	}

	if(res == PTHREAD_CANCELED){
		printf("thread was canceled\n");
	} else{
		printf("thread was not canceled.(this should not happen)\n");
	}

	return 0;
}
