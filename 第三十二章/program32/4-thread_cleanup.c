#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int glob = 0;

void cleanHandler(void *arg){
	printf("cleanup:  freeing block at %p\n",arg);
	free(arg);
	printf("cleanup:  unlocking mutex\n",arg);
	pthread_mutex_unlock(&mtx);
}

void *threadFunc(void *arg){
	void *buf;

	buf = malloc(0x1000);
	printf("thread:  allocated memory at %p\n",buf);

	pthread_mutex_lock(&mtx);

	pthread_cleanup_push(cleanHandler,buf);

	while(glob == 0){
		pthread_cond_wait(&cond,&mtx);
	}

	pthread_cleanup_pop(1);
}

int main(int argc,char *argv[]){
	pthread_t t;
	void *res;

	pthread_create(&t,NULL,threadFunc,NULL);

	sleep(2);

	if(argc == 1){
		printf("main:  about to cancel thread\n");
		pthread_cancel(t);
	}else{
		printf("main:  about to signal condition var\n");
		glob = 1;
		pthread_cond_signal(&cond);
	}

	pthread_join(t,&res);

	if(res == PTHREAD_CANCELED){
		printf("main:  thread was canceled\n");
	}else{
		printf("main:  thread terminated naomally\n");
	}

	return 0;
}
