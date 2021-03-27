#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

int main(int argc,char *argv[]){
	struct timeval timeout;
	struct timeval *tp;
	int num,i,ready,nfd,fd;
	char buf[10];
	fd_set readfds,writefds;

	nfd = 0;
	if(strcmp(argv[1],"-") == 0){
		tp = NULL;
	}else{
		tp = &timeout;
		tp->tv_sec = atoi(argv[1]);
		tp->tv_usec = 0;
	}

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	for(i = 2; i < argc; i++){
		num = sscanf(argv[i],"%d%2[rw]",&fd,buf);
		if(num != 2){
			fprintf(stderr,"sscanf\n");
			exit(EXIT_FAILURE);
		}

		if(fd >= nfd){
			nfd = fd + 1;
		}

		if(strchr(buf,'r') != NULL){
			FD_SET(fd,&readfds);
		}
		if(strchr(buf,'w') != NULL){
			FD_SET(fd,&writefds);
		}
	}

	ready = select(nfd,&readfds,&writefds,NULL,tp);
	if(ready == -1){
		fprintf(stderr,"select\n");
		exit(EXIT_FAILURE);
	}else if(ready == 0){
		printf("ready = %d\n",ready);
		fprintf(stderr,"timeout\n");
		exit(EXIT_FAILURE);
	}else{
		printf("ready = %d\n",ready);
		for(i = 0; i < nfd; i++){
			if(FD_ISSET(i,&readfds)){
				printf("fd:%d r\n",i);
			}else if(FD_ISSET(i,&writefds)){
				printf("fd:%d w\n",i);
			}
		}

		if(tp != NULL){
			printf("still left %ld sec and %ld macro sec\n",tp->tv_sec,tp->tv_usec);
		}
	}

	return 0;
}
