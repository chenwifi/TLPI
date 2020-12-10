#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MTEXT 1024

struct mbuf{
	long mtype;
	char mtext[MAX_MTEXT];
};

int main(int argc,char *argv[]){
	int opt;
	int flags,msqid,msglen;
	struct mbuf msg;

	flags = 0;
	while((opt = getopt(argc,argv,"n")) != -1){
		if(opt == 'n'){
			flags |= IPC_NOWAIT;
		}else{
			fprintf(stderr,"wrong args\n");
			exit(EXIT_FAILURE);
		}
	}

	if(argc < optind + 2 || argc > optind + 3){
		fprintf(stderr,"wrong num of args\n");
		exit(EXIT_FAILURE);
	}

	msqid = atoi(argv[optind]);
	msg.mtype = atoi(argv[optind + 1]);

	if(argc > optind + 2){
		msglen = strlen(argv[optind + 2]) + 1;
		if(msglen > MAX_MTEXT){
			fprintf(stderr,"msg too long\n");
			exit(EXIT_FAILURE);
		}
		memcpy(msg.mtext,argv[optind + 2],msglen);
	}else{
		msglen = 0;
	}

	if(msgsnd(msqid,&msg,msglen,flags) == -1){
		fprintf(stderr,"msgsnd\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
