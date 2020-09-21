#include <stdio.h>
#include <pthread.h>

void cl(void *arg){
}

int main(int argc,char *argv[]){

	int a = 4;
	int b = 1;

	if(b > 0){
		int a = 5;
		int c = 4;
		printf("a=%d\n",a);
	}

	int d = 6;

	if(1){
	}
	printf("a=%d\n",a);
	//printf("c=%d\n",c);
	printf("d=%d\n",d);

	pthread_cleanup_push(cl,NULL);
	pthread_cleanup_pop(0);

	return 0;
}
