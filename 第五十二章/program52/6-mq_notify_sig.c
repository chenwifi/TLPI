#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdlib.h>
#include <signal.h>

#define NOTIFY_SIG SIGUSR1

static void handler(int sig){
}

int main(int argc,char *argv[]){
	mqd_t mqd;
	void *buffer;
	struct mq_attr attr;
	sigset_t blockMask,emptyMask; 
	struct sigaction sa;
	struct sigevent sev;
	ssize_t numRead;

	if(argc != 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s mq-name\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	mqd = mq_open(argv[1],O_RDONLY | O_NONBLOCK);
	if(mqd == (mqd_t)-1){
		fprintf(stderr,"mq_open\n");
		exit(EXIT_FAILURE);
	}

	if(mq_getattr(mqd,&attr) == -1){
		fprintf(stderr,"mq_getattr\n");
		exit(EXIT_FAILURE);
	}
	buffer = malloc(attr.mq_msgsize);
	if(buffer == NULL){
		fprintf(stderr,"malloc\n");
		exit(EXIT_FAILURE);
	}

	sigemptyset(&blockMask);
	sigaddset(&blockMask,NOTIFY_SIG);
	sigprocmask(SIG_BLOCK,&blockMask,NULL);
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(NOTIFY_SIG,&sa,NULL);

	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = NOTIFY_SIG;
	if(mq_notify(mqd,&sev) == -1){
		fprintf(stderr,"mq_notify\n");
		exit(EXIT_FAILURE);
	}

	sigemptyset(&emptyMask);
	while(1){
		sigsuspend(&emptyMask);

		if(mq_notify(mqd,&sev) == -1){
			fprintf(stderr,"mq_notify\n");
			exit(EXIT_FAILURE);
		}

		while((numRead = mq_receive(mqd,buffer,attr.mq_msgsize,NULL)) >= 0){
			printf("Read %ld bytes\n",numRead);
		}

		if(errno != EAGAIN){
			fprintf(stderr,"mq_receive\n");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
