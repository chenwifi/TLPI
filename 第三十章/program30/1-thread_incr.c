#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int glob = 0;

static void *threadFunc(void *arg){
	int loop,i;
	int tmp;

	loop = (*(int *)arg);
	
	for(i = 0; i < loop; i++){
		tmp = glob;
		tmp++;
		glob = tmp;
	}

}

int main(int argc,char *argv[]){
	pthread_t t1,t2;
	int loop,s;

	loop = (argc > 1) ? atoi(argv[1]) : 10000000;

	s = pthread_create(&t1,NULL,threadFunc,&loop);
	if(s != 0){
		fprintf(stderr,"pthread_create\n");
		exit(0);
	}

	s = pthread_create(&t2,NULL,threadFunc,(void *)&loop);
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
