#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void *threadFunc(void *arg){
	char *str = (char *)arg;
	/*
	char tmp[3];
	tmp[0] = 'a';
	tmp[1] = 'b';
	tmp[2] = '\0';
	*/

	char *tmp = (char *)malloc(3);
	tmp[0] = 'a';
	tmp[1] = 'b';
	tmp[2] = '\0';


	//printf("%s\n",str);

	str = tmp;
	//return (void *)strlen(str);
	return str;
}

int main(int argc,char *argv[]){
	pthread_t t1;
	int s;
	void *res;
	
	s = pthread_create(&t1,NULL,threadFunc,"Hello world\n");

	s = pthread_join(t1,&res);
	printf("%s\n",(char *)res);

	return 0;
}
