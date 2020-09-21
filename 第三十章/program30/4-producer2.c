#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int avail = 0;

static void *producer(void *arg){
	int s;
	int a;
	a = *(int *)arg;

	while(1){

		while(avail < 10){
			s = pthread_mutex_lock(&mtx);
			//sleep(1);
			avail++;
			printf("produce 1 ,now avail=%d,from %d\n",avail,a);
			s = pthread_mutex_unlock(&mtx);
			sleep(1);
		}
		s = pthread_cond_broadcast(&cond);

		s = pthread_mutex_lock(&mtx);
		if(s != 0){
			fprintf(stderr,"pthread_mutex_lock\n");
			exit(0);
		}

		while(avail >= 10){
			s = pthread_cond_wait(&cond,&mtx);
			if(s != 0){
				fprintf(stderr,"pthread_mutex_lock\n");
				exit(0);
			}
		}

		s = pthread_mutex_unlock(&mtx);
		if(s != 0){
			fprintf(stderr,"pthread_mutex_lock\n");
			exit(0);
		}

		/*
		s = pthread_cond_broadcast(&cond);
		if(s != 0){
			fprintf(stderr,"pthread_mutex_lock\n");
			exit(0);
		}
		*/
	}
}

/*
static void *consumer(void *arg){
}
*/

int main(int argc,char *argv[]){
	pthread_t t1,t2;
	int s;
	int a,b;
	a = 1;
	b = 2;

	s = pthread_create(&t1,NULL,producer,&a);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}
	/*
	s = pthread_create(&t2,NULL,producer,&b);
	if(s != 0){
		fprintf(stderr,"pthread_mutex_lock\n");
		exit(0);
	}
	*/

	while(1){

		sleep(1);
		while(avail > 0){
			s = pthread_mutex_lock(&mtx);
			//sleep(5);
			printf("before consume avail=%d.consume product \n",avail);
			avail--;
			s = pthread_mutex_unlock(&mtx);
			sleep(5);
		}

		s = pthread_cond_broadcast(&cond);
		if(s != 0){
			fprintf(stderr,"pthread_mutex_lock\n");
			exit(0);
		}

		s = pthread_mutex_lock(&mtx);
		if(s != 0){
			fprintf(stderr,"pthread_mutex_lock\n");
			exit(0);
		}

		while(avail == 0){
			s = pthread_cond_wait(&cond,&mtx);
			if(s != 0){
				fprintf(stderr,"pthread_mutex_lock\n");
				exit(0);
			}
		}

		s = pthread_mutex_unlock(&mtx);
		if(s != 0){
			fprintf(stderr,"pthread_mutex_lock\n");
			exit(0);
		}

		/*
		s = pthread_cond_broadcast(&cond);
		if(s != 0){
			fprintf(stderr,"pthread_mutex_lock\n");
			exit(0);
		}
		*/
	}

	return 0;
}
