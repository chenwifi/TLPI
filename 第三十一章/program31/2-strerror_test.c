#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "1-mystrerror.h"

static void *threadFunc(void *arg){
	char *str;

	printf("Othre thread has call mystrerror()\n");
	str = mystrerror(EPERM);
	printf("Other thread : (%p) = %s\n",str,str);

	sleep(5);

	printf("Othre thread has call mystrerror()\n");
	str = mystrerror(EINTR);
	printf("Other thread : (%p) = %s\n",str,str);

	return NULL;
}

static void *threadFunc1(void *arg){
	char *str;

	printf("Othre thread has call mystrerror()\n");
	str = mystrerror(EACCES);
	//sleep(5);
	printf("Other thread : (%p) = %s\n",str,str);

	return NULL;
}

int main(int argc,char *argv[]){
	pthread_t t,t2;
	char *str;

	str = mystrerror(EINVAL);
	printf("Main thread has call mystrerror()\n");

	pthread_create(&t,NULL,threadFunc1,NULL);
	pthread_create(&t2,NULL,threadFunc1,NULL);
	pthread_join(t,NULL);
	pthread_join(t2,NULL);

	printf("Main thread : (%p) = %s\n",str,str);

	return 0;
}
