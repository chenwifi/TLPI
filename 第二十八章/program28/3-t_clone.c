#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef CHILD_SIG
#define CHILD_SIG SIGUSR1
#endif

struct test{
	int x;
	int *addr;
	int tfd;
};
typedef struct test clonestruct; 

static int childFunc(void *arg){
	clonestruct *tmp;
	tmp = (clonestruct *)arg;
	tmp->x = tmp->x * 3;
	printf("idata=%d addr=%p\n",tmp->x,tmp->addr);
	if(close(tmp->tfd) == -1){
		fprintf(stderr,"close error\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

int main(int argc,char *argv[]){
	const int STACK_SIZE = 65536;
	char *stack;
	char *stackTop;
	int fd,flags,a,s;
	clonestruct tmp;

	tmp.x = 100;
	tmp.addr = &a;


	fd = open("/dev/null",O_RDWR);
	if(fd == -1){
		fprintf(stderr,"open error\n");
		exit(EXIT_FAILURE);
	}

	tmp.tfd = fd;

	flags = (argc > 1) ? CLONE_FILES | CLONE_VM : 0;

	stack = (char *)malloc(STACK_SIZE);
	if(stack == NULL){
		fprintf(stderr,"malloc error\n");
		exit(EXIT_FAILURE);
	}
	stackTop = stack + STACK_SIZE;

	if(CHILD_SIG != 0 && CHILD_SIG != SIGCHLD){
		signal(CHILD_SIG,SIG_IGN);
	}

	if(clone(childFunc,stackTop,flags | CHILD_SIG,(void *)&tmp) == -1){
		fprintf(stderr,"clone error\n");
		exit(EXIT_FAILURE);
	}

	if(waitpid(-1,NULL,(CHILD_SIG != SIGCHLD) ? __WCLONE : 0) == -1){
		fprintf(stderr,"waitpid error\n");
		exit(EXIT_FAILURE);
	}
	printf("child has died\n");
	printf("idata a = %d,addr = %p\n",tmp.x,tmp.addr);

	s = write(tmp.tfd,"x",1);
	if(s == -1 && errno == EBADF){
		printf("fd %d has been closed\n",tmp.tfd);
	}else if(s == -1){
		printf("write on fail\n");
	}else{
		printf("fd %d success\n",tmp.tfd);
	}

	return 0;
}
