#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>

int main(int argc,char *argv[]){
	int flags,opt;
	mqd_t mqd;
	unsigned int prio;

	flags = O_WRONLY;
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

	if(optind + 1 >= argc){
		exit(EXIT_FAILURE);
	}

	mqd = mq_open(argv[optind],flags);
	if(mqd == (mqd_t)-1){
		exit(EXIT_FAILURE);
	}

	prio = (argc > optind + 2) ? atoi(argv[optind + 2]) : 0;

	if(mq_send(mqd,argv[optind + 1],strlen(argv[optind + 1]),prio) == -1){
		exit(EXIT_FAILURE);
	}

	return 0;
}
