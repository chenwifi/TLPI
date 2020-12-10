#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc,char *argv[]){
	int maxind,i,msqid;
	struct msginfo msginfo;
	struct msqid_ds ds;

	maxind = msgctl(0,MSG_INFO,(struct msqid_ds *)&msginfo);
	if(maxind == -1){
		fprintf(stderr,"msgctl\n");
		exit(EXIT_FAILURE);
	}

	printf("maxind: %d\n\n",maxind);
	printf("index	id     key		message\n");

	for(i = 0; i <= maxind; i++){
		msqid = msgctl(i,MSG_STAT,&ds);
		if(msqid == -1){
			if(errno != EINVAL && errno != EACCES){
				fprintf(stderr,"msgctl MSG_STAT\n");
				exit(EXIT_FAILURE);
			}
			continue;
		}

		printf("%4d %8d 0x%08lx %7ld\n",i,msqid,ds.msg_perm.__key,(long)ds.msg_qnum);
	}

	return 0;
}
