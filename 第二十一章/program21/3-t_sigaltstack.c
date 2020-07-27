#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void handler(int sig){
	int x;

	printf("caught signal\n");
	printf("Top of handler stack near  %10p\n",(void *)&x);

	_exit(EXIT_FAILURE);
}

static void overflowStack(int callNum){
	char a[1000000];

	printf("call %4d - top of stack near %10p\n",callNum,&a[0]);
	overflowStack(callNum+1);
}

int main(){
	stack_t sigstack;
	struct sigaction sa;
	int i;

	printf("Top of standard stack is near %10p\n",(void *)&i);

	sigstack.ss_sp = malloc(SIGSTKSZ);
	sigstack.ss_size = SIGSTKSZ;
	sigstack.ss_flags = 0;
	sigaltstack(&sigstack,NULL);

	printf("Alternate stack is at		%10p-%p",sigstack.ss_sp,(char *)sbrk(0)-1);

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sa.sa_flags = SA_ONSTACK;
	sigaction(SIGSEGV,&sa,NULL);

	overflowStack(1);

	return 0;
}
