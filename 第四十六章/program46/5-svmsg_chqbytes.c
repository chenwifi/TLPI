#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc,char *argv[]){
	struct msqid_ds ds;
	int msqid;

	if(argc != 3 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s msqid max-bytes\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	msqid = atoi(argv[1]);
	if(msgctl(msqid,IPC_STAT,&ds) == -1){
		fprintf(stderr,"msgctl\n");
		exit(EXIT_FAILURE);
	}

	printf("current max-bytes=%ud\n",ds.msg_qbytes);

	ds.msg_qbytes = atoi(argv[2]);
	if(msgctl(msqid,IPC_SET,&ds) == -1){
		fprintf(stderr,"msgctl \n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
