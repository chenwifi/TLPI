#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc,char *argv[]){
	pid_t childPid;
	int istack = 111;

	switch(childPid = vfork()){
		case -1 :
			break;
		case 0 :
			sleep(3);
			write(1,"child\n",7);
			istack = 333;
			//_exit(EXIT_SUCCESS);
			break;
		default :
			write(1,"parent\n",8);
			printf("istack=%d\n",istack);
			break;
	}

	return 0;
}
