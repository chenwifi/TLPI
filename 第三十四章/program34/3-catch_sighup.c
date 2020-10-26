#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void handler(int sig){
}

int main(int argc,char *argv[]){
	struct sigaction sa;
	pid_t childPid;

	setbuf(stdout,NULL);

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if(sigaction(SIGHUP,&sa,NULL) == -1){
		fprintf(stderr,"sigaction error\n");
		exit(EXIT_FAILURE);
	}

	childPid = fork();
	if(childPid < 0){
		fprintf(stderr,"fork error\n");
		exit(EXIT_FAILURE);
	}

	if(childPid == 0 && argc > 1){
		if(setpgid(0,0) == -1){
			fprintf(stderr,"setpgid\n");
			exit(EXIT_FAILURE);
		}
	}

	printf("PID=%ld; PPID=%ld; PGID=%ld; SID=%ld;\n",(long)getpid(),(long)getppid(),(long)getpgrp(),(long)getsid(0));

	alarm(60);

	while(1){
		pause();
		printf("%ld: caught SIGHUP\n",(long)getpid());
	}

	return 0;
}
