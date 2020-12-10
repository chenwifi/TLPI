#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void printWaitStatus(char *msg,int status){
	if(msg != NULL){
		printf("%s\n",msg);
	}

	if(WIFEXITED(status)){
		printf("child exited,status=%d\n",WEXITSTATUS(status));
	}else if(WIFSIGNALED(status)){
		printf("child killed by signal %d (%s)\n",WTERMSIG(status),strsignal(WTERMSIG(status)));
		if(WCOREDUMP(status)){
			printf("(core dump)");
		}
	}else if(WIFSTOPPED(status)){
		printf("child stopped by signal %d (%s)\n",WSTOPSIG(status),strsignal(WSTOPSIG(status)));
	}

#ifdef WIFCONTINUE
	else if(WIFCONTINUE(status)){
		printf("child continue");
	}
#endif

}
