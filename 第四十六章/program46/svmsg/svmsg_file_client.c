#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "svmsg_file.h"

static int clientId;

static void rmq(){
	if(msgctl(clientId,IPC_RMID,NULL) == -1){
		fprintf(stderr,"msgctl rmid\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc,char *argv[]){
	int serverId;
	struct requestMsg rqm;
	struct responseMsg rpm;
	ssize_t len;
	ssize_t cnt,bytes;

	clientId = msgget(IPC_PRIVATE,S_IRUSR | S_IWUSR | S_IWGRP);
	if(clientId == -1){
		fprintf(stderr,"msgget\n");
		exit(EXIT_FAILURE);
	}

	atexit(rmq);

	serverId = msgget(SERVER_KEY,S_IWUSR);
	if(serverId == -1){
		fprintf(stderr,"msgget\n");
		exit(EXIT_FAILURE);
	}

	rqm.mtype = 1;
	rqm.clientId = clientId;
	strncpy(rqm.pathname,argv[1],strlen(argv[1]) + 1);
	rqm.pathname[strlen(argv[1])] = '\0';

	if(msgsnd(serverId,&rqm,REQ_MSG_SIZE,0) == -1){
		fprintf(stderr,"msgsnd");
		exit(EXIT_FAILURE);
	}

	len = msgrcv(clientId,&rpm,RESP_MSG_SIZE,0,0);
	if(len == -1){
		fprintf(stderr,"msgrcv");
		exit(EXIT_FAILURE);
	}

	if(rpm.mtype == RESP_MT_FAILURE){
		printf("%s\n",rpm.data);
		exit(EXIT_FAILURE);
	}

	bytes = len;
	for(cnt = 1; rpm.mtype == RESP_MT_DATA; cnt++){
		len = msgrcv(clientId,&rpm,RESP_MSG_SIZE,0,0);
		if(len == -1){
			fprintf(stderr,"msgrcv");
			continue;
		}

		bytes += len;
	}

	printf("Received %ld bytes (%d messages)\n",bytes,cnt);

	return 0;
}
