#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>
#include "2-semun.h"

int main(int argc,char *argv[]){
	int semid,i;
	union semun arg,dummy;
	struct semid_ds ds;

	if(argc != 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s semid\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	semid = atoi(argv[1]);
	arg.buf = &ds;

	/*
	printf("%d\n",sizeof(arg.array[0]));
	exit(0);
	*/

	if(semctl(semid,0,IPC_STAT,arg) == -1){
		fprintf(stderr,"semctl\n");
		exit(EXIT_FAILURE);
	}

	printf("Semaphore changed: %s\n",ctime(&(ds.sem_ctime)));
	printf("Last semop(): %s\n",ctime(&(ds.sem_otime)));

	arg.array = calloc(ds.sem_nsems,sizeof(arg.array[0]));
	if(arg.array == NULL){
		fprintf(stderr,"calloc");
	}

	if(semctl(semid,0,GETALL,arg) == -1){
		fprintf(stderr,"semctl\n");
		exit(EXIT_FAILURE);
	}

	printf("Sem# Value SEMPID SEMNCNT SEMZCNT\n");

	for(i = 0; i < ds.sem_nsems; i++){
		printf("%3d  %5d %5d %5d %5d\n",i,arg.array[i],semctl(semid,i,GETPID,dummy),semctl(semid,i,GETNCNT,dummy),semctl(semid,i,GETZCNT,dummy));
	}

	return 0;
}
