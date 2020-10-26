#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){

	if(fork() != 0){
		_exit(EXIT_SUCCESS);
	}

	if(setsid() == -1){
		fprintf(stderr,"setsid\n");
		exit(EXIT_FAILURE);
	}

	fork();

	printf("PID=%ld,PGID=%ld,SID=%ld,tpgid=%ld\n",(long)getpid(),(long)getpgrp(),(long)getsid(0),(long)tcgetpgrp(STDIN_FILENO));

	if(open("/dev/tty",O_RDWR) == -1){
		fprintf(stderr,"open\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
