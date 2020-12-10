#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "svmsg_file.h"

static int serverId;

static void handler(int sig){
	int savedErrno;

	savedErrno = errno;
	while(waitpid(-1,NULL,WNOHANG) > 0){
		continue;
	}
	errno = savedErrno;
}

static void serveRequest(const struct requestMsg *req){
	int fd;
	struct responseMsg rpm;
	ssize_t len;

	if((fd = open(req->pathname,O_RDONLY)) == -1){
		fprintf(stderr,"open\n");
		rpm.mtype = RESP_MT_FAILURE;
		snprintf(rpm.data,RESP_MSG_SIZE,"open fail\n");
		if(msgsnd(req->clientId,&rpm,strlen(rpm.data),0) == -1){
			fprintf(stderr,"msgsnd\n");
			_exit(EXIT_FAILURE);
		}
	}

	rpm.mtype = RESP_MT_DATA;
	while((len = read(fd,rpm.data,RESP_MSG_SIZE)) > 0){
		if(msgsnd(req->clientId,&rpm,len,0) == -1){
			break;
		}
	}

	rpm.mtype = RESP_MT_END;
	msgsnd(req->clientId,&rpm,0,0);
}

void rmq(){
	if(msgctl(serverId,IPC_RMID,NULL) == -1){
		fprintf(stderr,"msgctl");
		exit(EXIT_FAILURE);
	}
}

int main(int argc,char *argv[]){
	struct sigaction sa;
	struct requestMsg rqm;
	pid_t pid;

	if((serverId = msgget(SERVER_KEY,IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IWGRP)) == -1){
		if(errno == EEXIST){
			serverId = msgget(SERVER_KEY,S_IRUSR | S_IWUSR | S_IWGRP);
			if(serverId == -1){
				fprintf(stderr,"msgget\n");
				exit(EXIT_FAILURE);
			}
		}else{
			fprintf(stderr,"msgget\n");
			exit(EXIT_FAILURE);
		}
	}

	atexit(rmq);

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
	sigaction(SIGCHLD,&sa,NULL);

	memset(&rqm,0,sizeof(struct requestMsg));
	
	while(1){
		if(msgrcv(serverId,&rqm,REQ_MSG_SIZE,0,0) == -1){
			if(errno == EINTR){
				continue;
			}else{
				fprintf(stderr,"msgrcv\n");
				break;
			}
		}

		switch(pid = fork()){
			case -1:
				fprintf(stderr,"fork\n");
				exit(EXIT_FAILURE);
				break;
			case 0:
				serveRequest(&rqm);
				_exit(EXIT_SUCCESS);
				break;
			default:
				break;
		}
	}

	if(msgctl(serverId,IPC_RMID,NULL) == -1){
		fprintf(stderr,"msgctl");
		exit(EXIT_FAILURE);
	}

	return 0;
}
