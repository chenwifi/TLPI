#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "print_wait_status.h"

#define LEN 200

int mySystem(char *command){
	int status;
	pid_t childPid;

	switch(childPid = fork()){
		case -1:
			return -1;
			break;
		case 0:
			execl("/bin/sh","sh","-c",command,(char *)NULL);
			_exit(127);
			break;
		default:
			if(waitpid(childPid,&status,0) == -1){
				return -1;
			}else{
				return status;
			}
			break;
	}
}

int main(int argc,char *argv[]){
	char buf[LEN];
	int status;

	while(1){
		printf("Command: ");
		fflush(stdout);
		
		fgets(buf,LEN,stdin);
		status = mySystem(buf);
		printf("system return: status=0x%04x(%d,%d)\n",status,status >> 8,status & 0x0f);
		
		if(status == -1){
			fprintf(stdout,"system error\n");
			exit(EXIT_SUCCESS);
		}else{
			if(WIFEXITED(status) && WEXITSTATUS(status) == 127){
				printf("probably could not invoke shell\n");
			}else{
				printWaitStatus(NULL,status);
			}
		}
	}

	return 0;
}
