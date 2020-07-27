#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include "signal_functions.h"

static volatile sig_atomic_t canJmp = 0;

#ifdef USE_SIGSETJMP
static sigjmp_buf senv;
#else
static jmp_buf env;
#endif

static void handler(int sig){
	printSigMask(stdout,"in handler\n");

	if(canJmp == 0){
		printf("env buffer not set yet\n");
		return ;
	}

#ifdef USE_SIGSETJMP
	siglongjmp(senv,1);
#else
	longjmp(env,1);
#endif

}

int main(int argc,char *argv[]){
	struct sigaction sa;

	printSigMask(stdout,"Signal mask at startup:\n");

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	sigaction(SIGINT,&sa,NULL);

#ifdef USE_SIGSETJMP
	printf("Calling sigsetjmp\n");
	if(sigsetjmp(senv,1) == 0)
#else
	printf("Calling setjmp\n");
	if(setjmp(env) == 0)
#endif
		canJmp = 1;
	else
		printSigMask(stdout,"After jump from handler\n");

	while(1){
		pause();
	}

	return 0;
}
