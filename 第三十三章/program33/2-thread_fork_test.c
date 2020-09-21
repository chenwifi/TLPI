#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>

void *threadFunc(void *arg){
	pid_t pid;
	printf("threadFunc: tid=%ld\n",(long)syscall(SYS_gettid));
	sleep(3);
	switch((pid = fork())){
		case -1:
			break;
		case 0:
			printf("child: tid=%ld\n",(long)syscall(SYS_gettid));
			break;
		default:
			printf("parent: tid=%ld\n",(long)syscall(SYS_gettid));
			waitpid(pid,NULL,0);
			break;
	}
}

void *threadFunc1(void *arg){
	printf("threadFunc1: tid=%ld\n",(long)syscall(SYS_gettid));
}

int main(int argc,char *argv[]){
	pthread_t t,t1;

	printf("main: tid=%ld\n",(long)syscall(SYS_gettid));

	pthread_create(&t,NULL,threadFunc,NULL);
	pthread_create(&t1,NULL,threadFunc1,NULL);
	pthread_join(t,NULL);
	pthread_join(t1,NULL);

	return 0;
}
