#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "2-print_wait_status.h"

int main(int argc,char *argv[]){
	pid_t childPid;
	int exitStatus,status;

	if(argc > 1 && strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"usage: %s [exit_status]\n",argv[0]);
	}

	switch(fork()){
		case -1:
			fprintf(stdout,"fork error\n");
			exit(EXIT_FAILURE);
			break;
		case 0:
			printf("Child start with PID:%ld\n",getpid());
			if(argc > 1){
				exitStatus = atoi(argv[1]);
				_exit(exitStatus);
			}else{
				while(1){
					pause();
				}
				_exit(EXIT_FAILURE);
			}
			break;
		default:
			while(1){
				childPid = waitpid(-1,&status,WUNTRACED | WCONTINUED);
				printf("waitpid return: PID=%ld; status=0x%04x (%d %d)\n",(long)childPid,(unsigned int)status,status >> 8,status & 0xff);
				printWaitStatus(NULL,status);

				if(WIFEXITED(status) || WIFSIGNALED(status)){
					exit(EXIT_SUCCESS);
				}
			}
			break;
	}

	return 0;
}
