#include <stdio.h>
#include <signal.h>

typedef void (*sighandler)(int);

sighandler mysignal(int sig,sighandler handler){
	struct sigaction newDisp,prevDisp;

	sigemptyset(&newDisp.sa_mask);
	newDisp.sa_handler = handler;

#ifdef OLD_SIGNAL
	newDisp.sa_flags = SA_RESETHAND | SA_NODEFER;
#else
	newDisp.sa_flags = SA_RESTART;
#endif

	if(sigaction(sig,&newDisp,&prevDisp) != 0){
		return SIG_ERR;
	}else{
		return prevDisp.sa_handler;
	}
}

int main(){

	return 0;
}
