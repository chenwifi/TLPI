#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <mqueue.h>

int main(int argc,char *argv[]){
	mqd_t mqd;
	struct mq_attr attr;

	if(argc != 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s mq-name\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	mqd = mq_open(argv[1],O_RDONLY);
	if(mqd == (mqd_t) -1){
		fprintf(stderr,"mq_open:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	if(mq_getattr(mqd,&attr) == -1){
		fprintf(stderr,"mq_getattr\n");
		exit(EXIT_FAILURE);
	}

	printf("Maxinum of messages on queue: %ld\n",attr.mq_maxmsg);
	printf("Maxinum  messages size : %ld\n",attr.mq_msgsize);
	printf("message current on queue: %ld\n",attr.mq_curmsgs);

	return 0;
}
