#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int glob = 0;

static void *func1(void *arg){
	int a = (*(int *)arg);
	sleep(3);

	pthread_mutex_lock(&mtx);
	glob++;
	pthread_mutex_unlock(&mtx);

	sleep(3);
	printf("before signal: %d\n",a);

	//pthread_cond_broadcast(&cond);
	pthread_cond_signal(&cond);
	pthread_cond_signal(&cond);
}

static void *func2(void *arg){
	int a = (*(int *)arg);

	pthread_mutex_lock(&mtx);
	printf("before wait: %d\n",a);
	pthread_cond_wait(&cond,&mtx);
	glob++;
	printf("glob added: %d\n",a);
	pthread_mutex_unlock(&mtx);
}

int main(int argc,char *argv[]){
	pthread_t t1,t2,t3;
	int a,b,c;

	a = 1;
	b = 2;
	c = 3;

	pthread_create(&t1,NULL,func1,&a);
	pthread_create(&t2,NULL,func2,&b);
	pthread_create(&t3,NULL,func2,&c);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);

	printf("glob is %d\n",glob);

	return 0;
}
