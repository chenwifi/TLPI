#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MTEXT 1024

struct mbuf{
	long mtype;
	char mtext[MAX_MTEXT];
};

int main(int argc,char *argv[]){
	int type,flags,msqid;
	int opt;
	ssize_t msgLen;
	size_t maxBytes;
	struct mbuf msg;

	flags = 0;
	type = 0;
	while((opt = getopt(argc,argv,"ent:x")) != -1){
		switch(opt){
			case 'e':
				flags |= MSG_NOERROR;
				break;
			case 'n':
				flags |= IPC_NOWAIT;
				break;
			case 't':
				type = atoi(optarg);
				break;
#ifdef MSG_EXCEPT
			case 'x':
				flags |= MSG_EXCEPT;
				break;
#endif
			default:
				fprintf(stderr,"getopt\n");
				break;
		}
	}

	if(argc < optind + 1 || argc > optind + 2){
		fprintf(stderr,"wrong num of args");
		exit(EXIT_FAILURE);
	}

	msqid = atoi(argv[optind]);
	maxBytes = (argc > optind + 1) ? atoi(argv[optind + 1]) : MAX_MTEXT;

	msgLen = msgrcv(msqid,&msg,maxBytes,type,flags);
	if(msgLen == -1){
		fprintf(stderr,"msgrcv\n");
		exit(EXIT_FAILURE);
	}

	printf("Received: type=%ld; length=%ld ",msg.mtype,(long)msgLen);
	if(msgLen > 0){
		printf("; body=%s\n",msg.mtext);
	}

	return 0;
}
