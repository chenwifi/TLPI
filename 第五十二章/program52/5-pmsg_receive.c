#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>

int main(int argc,char *argv[]){
	int flags,opt;
	mqd_t mqd;
	struct mq_attr attr;
	void *buffer;
	ssize_t numRead;
	unsigned int prot;

	flags = O_RDONLY;
	while((opt = getopt(argc,argv,"n")) != -1){
		switch(opt){
			case 'n':
				flags |= O_NONBLOCK;
				break;
			default:
				exit(EXIT_FAILURE);
				break;
		}
	}

	if(optind >= argc){
		exit(EXIT_FAILURE);
	}

	mqd = mq_open(argv[optind],flags);
	if(mqd == (mqd_t)-1){
		exit(EXIT_FAILURE);
	}

	if(mq_getattr(mqd,&attr) == -1){
		exit(EXIT_FAILURE);
	}

	buffer = malloc(attr.mq_msgsize);
	if(buffer == NULL){
		exit(EXIT_FAILURE);
	}

	numRead = mq_receive(mqd,buffer,attr.mq_msgsize,&prot);
	if(numRead == -1){
		exit(EXIT_FAILURE);
	}

	printf("Read %ld bytes;prio = %u\n",numRead,prot);
	if(write(STDOUT_FILENO,buffer,numRead) != numRead){
		exit(EXIT_FAILURE);
	}
	write(STDOUT_FILENO,"\n",1);

	return 0;
}
