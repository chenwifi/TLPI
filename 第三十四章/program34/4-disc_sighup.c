#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static void handler(int sig){
	printf("PID %ld: caught signal %2d (%s)\n",(long)getpid(),sig,strsignal(sig));
}

int main(int argc,char *argv[]){
	pid_t parentId,childId;
	int i;
	struct sigaction sa;

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s {d|s} ...\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	setbuf(stdout,NULL);

	parentId = getpid();
	printf("PID of parent process is: %ld\n",(long)parentId);
	printf("Foregroup process group ID is: %ld\n",(long)tcgetpgrp(STDIN_FILENO));

	for(i = 1; i < argc; i++){
		childId = fork();
		if(childId == -1){
			fprintf(stderr,"fork\n");
			exit(EXIT_FAILURE);
		}

		if(childId == 0){
			if(argv[i][0] == 'd'){
				if(setpgid(0,0)){
					fprintf(stderr,"setpgid\n");
					exit(EXIT_FAILURE);
				}
			}

			sa.sa_flags = 0;
			sigemptyset(&sa.sa_mask);
			sa.sa_handler = handler;
			if(sigaction(SIGHUP,&sa,NULL) == -1){
				fprintf(stderr,"sigaction\n");
				exit(EXIT_FAILURE);
			}

			break;
		}
	}

	alarm(60);

	printf("PID=%ld; PGID=%ld\n",(long)getpid(),(long)getpgrp());

	while(1){
		pause();
	}

	return 0;
}
