#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <string.h>
#include <stdlib.h>

#define MAX_EVENTS 5
#define MAX_BUF 1000

int main(int argc,char *argv[]){
	int epfd,i,fd,ready,numOpenFds,numReads;
	struct epoll_event event;
	struct epoll_event evlist[MAX_EVENTS];
	char buf[MAX_BUF];

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s file...\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	epfd = epoll_create(argc - 1);
	if(epfd == -1){
		fprintf(stderr,"epoll_create\n");
		exit(EXIT_FAILURE);
	}

	for(i = 1; i < argc; i++){
		fd = open(argv[i],O_RDONLY);
		if(fd == -1){
			fprintf(stderr,"open\n");
			exit(EXIT_FAILURE);
		}
		printf("open on fd:%d\n",fd);

		event.events = EPOLLIN;
		event.data.fd = fd;
		if(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event) == -1){
			fprintf(stderr,"epoll_ctl\n");
			exit(EXIT_FAILURE);
		}
	}

	numOpenFds = argc - 1;
	while(numOpenFds > 0){
		ready = epoll_wait(epfd,evlist,MAX_EVENTS,-1);
		if(ready == -1){
			if(errno == EINTR){
				continue;
			}else{
				fprintf(stderr,"epoll_wait\n");
				exit(EXIT_FAILURE);
			}
		}

		printf("Ready: %d\n",ready);

		for(i = 0; i < ready; i++){
			printf("fd = %d;events: %s%s%s\n",evlist[i].data.fd,(evlist[i].events & EPOLLIN) ? " EPOLLIN" : "",(evlist[i].events & EPOLLHUP) ? " EPOLLHUP" : "",(evlist[i].events & EPOLLERR) ? " EPOLLERR" : "");

			if(evlist[i].events & EPOLLIN){
				numReads = read(evlist[i].data.fd,buf,MAX_BUF);
				if(numReads < 0){
					fprintf(stderr,"read\n");
					exit(EXIT_FAILURE);
				}

				printf("read %d bytes: %.*s\n",numReads,numReads,buf);

			}else if(evlist[i].events & (EPOLLHUP | EPOLLERR)){
				printf("closing fd %d\n",evlist[i].data.fd);
				close(evlist[i].data.fd);
				numOpenFds--;
			}
		}
	}

	printf("all file closed\n");


	return 0;
}
