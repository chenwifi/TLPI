#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc,char *argv[]){
	int i,msqid;

	if(argc > 1 && strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s msqid\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	for(i = 1; i < argc; i++){
		msqid = atoi(argv[i]);
		if(msgctl(msqid,IPC_RMID,NULL) == -1){
			fprintf(stderr,"msgctl %s",argv[i]);
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
