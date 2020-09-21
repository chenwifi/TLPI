#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

static void *threadFunc(void *arg){
	char *str = (char *)arg;

	printf("%s\n",str);

	return (void *)strlen(str);
}

int main(int argc,char *argv[]){
	pthread_t t1;
	int s;
	void *res;

	s = pthread_create(&t1,NULL,threadFunc,"Hello world\n");
	if(s != 0){
		fprintf(stderr,"pthread_create error\n");
		exit(EXIT_FAILURE);
	}
	printf("msg from main\n");

	s = pthread_join(t1,&res);
	if(s != 0){
		fprintf(stderr,"pthread_join error\n");
		exit(EXIT_FAILURE);
	}

	printf("thread return %ld\n",(long)res);

	return 0;
}
