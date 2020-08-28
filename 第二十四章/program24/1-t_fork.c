#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int idata = 111;

int main(){
	pid_t childPid;
	int istack = 222;

	switch(childPid = fork()){
		case 0:
			idata = 333;
			istack = 444;
			break;
		case -1:
			fprintf(stderr,"fork\n");
			exit(EXIT_FAILURE);
			break;
		default:
			sleep(3);
			break;
	}

	printf("PID=%ld %s idata=%d istack=%d\n",(long)getpid(),(childPid == 0) ? "CHILD" : "PARENT",idata,istack);

	return 0;
}
