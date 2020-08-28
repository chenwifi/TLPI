#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
	int i,numDead;
	pid_t cpid,tmppid;
	int status;

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stdout,"usage: %s sleep_time ...\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	setbuf(stdout,NULL);

	for(i = 1; i < argc; i++){
		switch(cpid = fork()){
			case -1:
				fprintf(stdout,"fork error\n");
				exit(EXIT_FAILURE);
				break;
			case 0:
				printf("child %d started with PID %ld,sleeping %s seconds\n",i,getpid(),argv[i]);
				sleep(atoi(argv[i]));
				_exit(i);
				break;
			default:
				break;

		}
	}

	numDead = 0;
	while(1){
		if((tmppid = wait(&status)) == -1){
			if(errno == ECHILD){
				printf("No more children\n");
				exit(EXIT_SUCCESS);
			}else{
				fprintf(stdout,"wait error\n");
				exit(EXIT_FAILURE);
			}
		
		}

		numDead++;
		printf("wait() return child PID %ld (numDead=%d) status=%d\n",tmppid,numDead,status);
	}

	return 0;
}
