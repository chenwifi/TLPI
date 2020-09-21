#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#define handle_error_en(en,msg) \
	do {errno = en; perror(msg); exit(EXIT_FAILURE);} while(0)

static void *threadFunc(void *arg){
	int s;

	s = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
	if(s != 0){
		handle_error_en(s,"setcanstate");
	}

	printf("threadfun: cancellation disabled\n");
	sleep(5);
	printf("threadfun: enable\n");

	s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	if(s != 0){
		handle_error_en(s,"setcanstate");
	}

	sleep(1000);

	printf("threadfun: could not show\n");

	return NULL;
}

int main(int argc,char *argv[]){
	int s;
	pthread_t t;
	void *res;

	s = pthread_create(&t,NULL,threadFunc,NULL);
	if(s != 0){
		handle_error_en(s,"pthread_create");
	}

	sleep(2);
	printf("main(): sending cancellation request\n");

	s = pthread_cancel(t);
	if(s != 0){
		handle_error_en(s,"pthread_cancel");
	}

	s = pthread_join(t,&res);
	if(s != 0){
		handle_error_en(s,"pthread_join");
	}

	if(res == PTHREAD_CANCELED){
		printf("main(): thread was cancel\n");
	}else{
		printf("should not happen\n");
	}

	return 0;
}
