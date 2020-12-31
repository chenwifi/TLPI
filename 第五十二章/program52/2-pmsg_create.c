#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

int main(int argc,char *argv[]){
	struct mq_attr attr,*attrp;
	int opt,flags;
	mode_t perms;
	mqd_t mqd;

	flags = O_RDWR;
	attr.mq_maxmsg = 50;
	attr.mq_msgsize = 2048;
	attrp = NULL;

	while((opt = getopt(argc,argv,"cm:s:x")) != -1){
		switch(opt){
			case 'c':
				flags |= O_CREAT;
				break;
			case 'm':
				attr.mq_maxmsg = atoi(optarg);
				attrp = &attr;
				break;
			case 's':
				attr.mq_msgsize = atoi(optarg);
				attrp = &attr;
				break;
			case 'x':
				flags |= O_EXCL;
				break;
			default:
				exit(EXIT_FAILURE);
				break;
		}
	}

	if(optind >= argc){
		exit(EXIT_FAILURE);
	}

	perms = (optind + 1 >= argc) ? (S_IRUSR | S_IWUSR) : strtol(argv[optind + 1],NULL,8);

	mqd = mq_open(argv[optind],flags,perms,attrp);
	if(mqd == (mqd_t)-1){
		fprintf(stderr,"mq_open\n");
		exit(EXIT_FAILURE);
	}

	printf("successful\n");

	return 0;
}
