#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc,char *argv[]){
	int i,shmid,flags;
	char *p,*retAddr;
	void *addr;

	printf("SHMLBA = %ld (%#lx),PID = %ld\n",(long)SHMLBA,(long)SHMLBA,(long)getpid());

	flags = 0;
	for(i = 1; i < argc; i++){
		shmid = strtol(argv[i],&p,0);
		if(*p != ':'){
			exit(EXIT_FAILURE);
		}

		addr = (void *)strtol(p + 1,NULL,0);
		if(strchr(p + 1,'r') != NULL){
			flags |= SHM_RND;
		}
		if(strchr(p + 1,'R') != NULL){
			flags |= SHM_RDONLY;
		}

		retAddr = shmat(shmid,addr,flags);
		if(retAddr == (void *)-1){
			exit(EXIT_FAILURE);
		}

		printf("%d: %s ==> %p\n",i,argv[i],retAddr);
	}

	printf("Sleeping 5 secs\n");
	sleep(5);

	return 0;
}
