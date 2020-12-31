#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

static void notifySetup(mqd_t *mqd);

static void threadFunc(union sigval sv){
	mqd_t *mqdp;
	struct mq_attr attr;
	void *buffer;
	ssize_t numRead;

	printf("PID: %ld and TID: %ld\n",(long)getpid(),syscall(SYS_gettid));

	mqdp = sv.sival_ptr;
	if(mq_getattr(*mqdp,&attr) == -1){
		fprintf(stderr,"mq_getattr\n");
		exit(EXIT_FAILURE);
	}
	buffer = malloc(attr.mq_msgsize);
	if(buffer == NULL){
		fprintf(stderr,"malloc\n");
		exit(EXIT_FAILURE);
	}

	notifySetup(mqdp);

	while((numRead = mq_receive(*mqdp,buffer,attr.mq_msgsize,NULL)) >= 0){
		printf("Read %ld bytes\n",numRead);
	}

	if(errno != EAGAIN){
		fprintf(stderr,"mq_receive\n");
		exit(EXIT_FAILURE);
	}

	free(buffer);
}

static void notifySetup(mqd_t *mqd){
	struct sigevent sev;
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = threadFunc;
	sev.sigev_notify_attributes = NULL;
	sev.sigev_value.sival_ptr = mqd;

	if(mq_notify(*mqd,&sev) == -1){
		fprintf(stderr,"mq_notify\n");
		exit(EXIT_FAILURE);
	}
	printf("successful\n");
}

int main(int argc,char *argv[]){
	mqd_t mqd;
	int a = 2;

	if(argc != 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s mq-name\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK);
	if(mqd == (mqd_t)-1){
		fprintf(stderr,"mq_open\n");
		exit(EXIT_FAILURE);
	}

	notifySetup(&mqd);
	pause();

	return 0;
}
