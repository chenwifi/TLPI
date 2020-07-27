#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void handler(int sig){
	printf("caught signal\n");
}

int main(int argc,char *argv[]){
	struct sigaction sa;
	unsigned int secs;
	ssize_t numReads;
	char buf[1024];
	int savedErrno;

	if(argc > 1 && strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"usage: %s [num-secs [restart-flag]]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	sa.sa_flags = (argc > 2) ? SA_RESTART : 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(SIGALRM,&sa,NULL);

	secs = (argc > 1) ? atoi(argv[1]) : 10;
	alarm(secs);
	numReads = read(0,buf,1024);
	savedErrno = errno;
	alarm(0);
	errno = savedErrno;

	if(numReads == -1){
		if(errno == EINTR){
			printf("read time out\n");
		}else{
			fprintf(stderr,"read error\n");
			exit(EXIT_FAILURE);
		}
	}else{
		printf("read %d : %s\n",numReads,buf);
	}

	return 0;
}
