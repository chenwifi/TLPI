#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "2-semun.h"

#define KEY 0x1aaaa1

int main(int argc,char *argv[]){
	int semid;
	union semun arg;
	struct sembuf sop;
	const int MAX_TRIES = 10;
	struct semid_ds ds;
	int i;

	semid = semget(KEY,1,IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	if(semid != -1){

		arg.val = 0;
		if(semctl(semid,0,SETVAL,arg) == -1){
			fprintf(stderr,"semctl\n");
			exit(EXIT_FAILURE);
		}

		sop.sem_num = 0;
		sop.sem_op = 0;
		sop.sem_flg = 0;

		if(semop(semid,&sop,1) == -1){
			fprintf(stderr,"semop\n");
			exit(EXIT_FAILURE);
		}
	}else{
		if(errno != EEXIST){
			exit(EXIT_FAILURE);
		}

		semid = semget(KEY,1,S_IRUSR | S_IWUSR);
		if(semid == -1){
			fprintf(stderr,"semget\n");
			exit(EXIT_FAILURE);
		}

		arg.buf = &ds;

		for(i = 0; i < MAX_TRIES; i++){
			if(semctl(semid,0,IPC_STAT,arg) == -1){
				fprintf(stderr,"semctl\n");
				exit(EXIT_FAILURE);
			}

			if(ds.sem_otime != 0){
				break;
			}

			sleep(1);
		}

		if(ds.sem_otime == 0){
			fprintf(stderr,"sem not init\n");
			exit(EXIT_FAILURE);
		}

		printf("now get the init sem\n");
	}

	return 0;
}
