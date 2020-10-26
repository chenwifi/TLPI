#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

static void handler(int sig){
	printf("PID=%ld: caught signal %d (%s)\n",(long)getpid(),sig,strsignal(sig));
}

int main(int argc,char *argv[]){
	struct sigaction sa;
	int i;

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s {s|p}...\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	setbuf(stdout,NULL);

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if(sigaction(SIGCONT,&sa,NULL) == -1){
		fprintf(stderr,"sigaction\n");
		exit(EXIT_FAILURE);
	}
	if(sigaction(SIGHUP,&sa,NULL) == -1){
		fprintf(stderr,"sigaction\n");
		exit(EXIT_FAILURE);
	}
	if(sigaction(SIGTSTP,&sa,NULL) == -1){
		fprintf(stderr,"sigaction\n");
		exit(EXIT_FAILURE);
	}

	printf("parent: PID=%ld,PPID=%ld,PGID=%ld,SID=%ld\n",(long)getpid(),(long)getppid(),(long)getpgrp(),(long)getsid(0));

	for(i = 1; i < argc; i++){
		switch(fork()){
			case -1:
				fprintf(stderr,"fork\n");
				exit(EXIT_FAILURE);
				break;
			case 0:
				printf("child: PID=%ld,PPID=%ld,PGID=%ld,SID=%ld\n",(long)getpid(),(long)getppid(),(long)getpgrp(),(long)getsid(0));
				if(argv[i][0] == 's'){
					printf("PID=%ld: stopping\n",(long)getpid());
					raise(SIGSTOP);
				}else{
					alarm(60);
					printf("PID=%ld: pausing\n",(long)getpid());
					while(1){
						pause();
					}
				}

				_exit(EXIT_SUCCESS);

				break;
			default:
				break;
		}
	}

	sleep(5);
	printf("parent existing\n");

	return 0;
}
