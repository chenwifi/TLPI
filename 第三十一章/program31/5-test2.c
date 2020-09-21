#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "1-mystrerror.h"

static void *threadFunc(void *arg){
	int a = 5;


	printf("Other thread : (%p) = %d\n",a,a);

	return NULL;
}

static void *threadFunc1(void *arg){
	int b = 2;

	printf("Other thread : (%p) = %d\n",b,b);

	return NULL;
}

int main(int argc,char *argv[]){
	pthread_t t,t2;
	char *str;

	pthread_create(&t,NULL,threadFunc,NULL);
	pthread_create(&t2,NULL,threadFunc1,NULL);
	pthread_join(t,NULL);
	pthread_join(t2,NULL);

	return 0;
}
