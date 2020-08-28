#define _GNU_SOURCE 
#include <stdio.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>

static int childFunc(void *arg){
	printf("from child tid=%ld PID=%ld PPID=%ld\n",(long)syscall(SYS_gettid),(long)getpid(),(long)getppid());

	sleep(7);

	return 0;
}

int main(int argc,char *argv[]){
	const int STACK_SIZE = 65536;
	char *stack;
	char *stackTop;
	int flags;

	stack = (char *)malloc(STACK_SIZE);
	if(stack == NULL){
		fprintf(stderr,"malloc error\n");
		exit(EXIT_FAILURE);
	}
	stackTop = stack + STACK_SIZE;

	flags = CLONE_THREAD | CLONE_SIGHAND | CLONE_VM | CLONE_FS | CLONE_FILES;

	if(clone(childFunc,stackTop,flags,NULL) == -1){
		fprintf(stderr,"clone error\n");
		exit(EXIT_FAILURE);
	}
	sleep(5);

	printf("from parent tid=%ld PID=%ld PPID=%ld\n",(long)syscall(SYS_gettid),(long)getpid(),(long)getppid());

	return 0;
}
