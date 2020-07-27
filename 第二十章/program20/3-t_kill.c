#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc,char *argv[]){
	int s,sig,pid;

	if(argc != 3 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s pid signum\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	sig = atoi(argv[2]);
	pid = atoi(argv[1]);

	s = kill(pid,sig);
	if(sig != 0){
		if(s != 0){
			fprintf(stderr,"kill");
			exit(EXIT_FAILURE);
		}
	}else{
		if(s == 0){
			printf("we can send signals to this process\n");
		}else{
			if(errno == EPERM){
				printf("the process is exists but we have no permission \n");
			}else if(errno == ESRCH){
				printf("the process is not exists\n");
			}
		}
	}

	return 0;
}
