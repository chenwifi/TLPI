#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

static void *threadFunc(void *arg){
	int a;

	a = *((int *)arg);

	printf("arg is %d\n",a);

	return ;
}

int main(int argc,char *argv[]){
	pthread_t t1[2];
	int i;

	for(i = 0; i < 2; i++){
		pthread_create(t1 + i,NULL,threadFunc,&i);
	}

	pthread_join(t1[0],NULL);
	pthread_join(t1[1],NULL);

	return 0;
}
