#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>
#include "2-semun.h"

int main(int argc,char *argv[]){
	union semun arg;
	struct semid_ds ds;
	int semid,i;

	if(argc < 3 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s semid val...\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	semid = atoi(argv[1]);

	arg.buf = &ds;
	if(semctl(semid,0,IPC_STAT,arg) == -1){
		fprintf(stderr,"semctl\n");
		exit(EXIT_FAILURE);
	}

	if(argc - 2 != ds.sem_nsems){
		fprintf(stderr,"argc wrong\n");
		exit(EXIT_FAILURE);
	}

	arg.array = calloc(ds.sem_nsems,sizeof(arg.array[0]));
	if(arg.array == NULL){
		fprintf(stderr,"calloc\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < ds.sem_nsems; i++){
		arg.array[i] = atoi(argv[i + 2]);
	}

	if(semctl(semid,0,SETALL,arg) == -1){
		fprintf(stderr,"semctl\n");
		exit(EXIT_FAILURE);
	}

	printf("Semaphore values changed (PID=%ld)\n",(long)getpid());

	return 0;
}
