#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fifo_seqnum.h"

int main(int argc,char *argv[]){
	int fd,clientfd;
	struct request rq;
	struct response rp;
	int seqNum = 0;
	char buf[CLIENT_FIFO_NAME_LEN];

	umask(0);
	if(mkfifo(SERVER_FIFO,S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST){
		fprintf(stderr,"mkfifo\n");
		exit(EXIT_FAILURE);
	}

	fd = open(SERVER_FIFO,O_RDONLY);
	if(fd == -1){
		fprintf(stderr,"open\n");
		exit(EXIT_FAILURE);
	}

	if(open(SERVER_FIFO,O_WRONLY) ==  -1){
		fprintf(stderr,"open\n");
		exit(EXIT_FAILURE);
	}

	signal(SIGPIPE,SIG_IGN);

	while(1){
		if(read(fd,&rq,sizeof(struct request)) != sizeof(struct request)){
			fprintf(stderr,"read\n");
			continue;
		}

		snprintf(buf,CLIENT_FIFO_NAME_LEN,CLIENT_FIFO_TEMPLATE,(long)rq.pid);
		clientfd = open(buf,O_WRONLY); 
		if(clientfd == -1){
			fprintf(stderr,"open \n");
			continue;
		}

		rp.seqNum = seqNum;
		seqNum += rq.seqLen;
		printf("%d---%d\n",seqNum,rp.seqNum);

		if(write(clientfd,&rp,sizeof(struct response)) != sizeof(struct response)){
			fprintf(stderr,"write\n");
			continue;
		}

		close(clientfd);
	}

	return 0;
}
