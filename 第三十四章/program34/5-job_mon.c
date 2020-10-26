#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static int cmdNum;

static void handler(int sig){
	if(getpid() == getpgrp()){
		fprintf(stderr,"Terminal FG process group: %ld\n",(long)tcgetpgrp(STDIN_FILENO));
	}

	fprintf(stderr,"Process %ld (%d) received signal %d (%s)",(long)getpid(),cmdNum,sig,strsignal(sig));

	if(sig == SIGTSTP){
		raise(SIGSTOP);
	}
}

int main(int argc,char *argv[]){
	struct sigaction sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if(sigaction(SIGINT,&sa,NULL) == -1){
		fprintf(stderr,"sigaction\n");
		exit(EXIT_FAILURE);
	}
	if(sigaction(SIGTSTP,&sa,NULL) == -1){
		fprintf(stderr,"sigaction\n");
		exit(EXIT_FAILURE);
	}
	if(sigaction(SIGCONT,&sa,NULL) == -1){
		fprintf(stderr,"sigaction\n");
		exit(EXIT_FAILURE);
	}

	if(isatty(STDIN_FILENO)){
		fprintf(stderr,"Terminal FG process group: %ld\n",(long)tcgetpgrp(STDIN_FILENO));
		fprintf(stderr,"Command  PID  PPID  PGRP  SID\n");
		cmdNum = 0;
	}else{
		if(read(STDIN_FILENO,&cmdNum,sizeof(cmdNum)) <= 0){
			fprintf(stderr,"read\n");
			exit(EXIT_FAILURE);
		}
	}

	cmdNum++;
	fprintf(stderr,"%4d  %5ld  %5ld %5ld %5ld\n",cmdNum,(long)getpid(),(long)getppid(),(long)getpgrp(),(long)getsid(0));

	if(!isatty(STDOUT_FILENO)){
		if(write(STDOUT_FILENO,&cmdNum,sizeof(cmdNum)) <= 0){
			fprintf(stderr,"write\n");
			exit(EXIT_FAILURE);
		}
	}

	while(1){
		pause();
	}

	return 0;
}
