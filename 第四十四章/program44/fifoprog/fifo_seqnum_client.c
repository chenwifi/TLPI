#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "fifo_seqnum.h"

static char buf[CLIENT_FIFO_NAME_LEN];

void handler(){
	unlink(buf);
}

int main(int argc,char *argv[]){
	int fd;
	struct request rq;
	struct response rp;
	pid_t pid;

	if(argc > 1 && strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s seqnum\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	pid = getpid();
	umask(0);
	snprintf(buf,CLIENT_FIFO_NAME_LEN,CLIENT_FIFO_TEMPLATE,(long)pid);
	if(mkfifo(buf,S_IRUSR | S_IWUSR | S_IWGRP) == -1){
		fprintf(stderr,"mkfifo\n");
		exit(EXIT_FAILURE);
	}
	atexit(handler);

	fd = open(SERVER_FIFO,O_WRONLY);
	if(fd == -1){
		fprintf(stderr,"open\n");
		exit(EXIT_FAILURE);
	}

	rq.pid = pid;
	rq.seqLen = (argc >= 2) ? atoi(argv[1]) : 1;

	if(write(fd,&rq,sizeof(struct request)) != sizeof(struct request)){
		fprintf(stderr,"write\n");
		exit(EXIT_FAILURE);
	}

	close(fd);

	if((fd = open(buf,O_RDONLY)) == -1){
		fprintf(stderr,"open\n");
		exit(EXIT_FAILURE);
	}

	sleep(10);

	if(read(fd,&rp,sizeof(struct response)) != sizeof(struct response)){
		fprintf(stderr,"read\n");
		exit(EXIT_FAILURE);
	}

	printf("receive seqnum is %d\n",rp.seqNum);

	return 0;
}
