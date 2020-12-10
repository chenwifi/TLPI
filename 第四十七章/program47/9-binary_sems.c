#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "2-semun.h"
#include "9-binary_sems.h"

int initSemAvailable(int semId,int semNum){
	union semun arg;

	arg.val = 1;
	return semctl(semId,semNum,SETVAL,arg); 

}

int initSemInUse(int semId,int semNum){
	union semun arg;

	arg.val = 0;
	return semctl(semId,semNum,SETVAL,arg); 
}

int reserveSem(int semId,int semNum){
	struct sembuf sop;
	int flags;

	flags = 0;
	if(bsUseSemUndo){
		flags |= SEM_UNDO;
	}else if(bsNoWait){
		flags |= IPC_NOWAIT;
	}
	sop.sem_num = semNum;
	sop.sem_flg = flags;
	sop.sem_op = -1;

	while(semop(semId,&sop,1) == -1){
		if(errno != EINTR && !bsRetryOnEintr){
			return -1;
		}
	}

	return 0;
}

int releaseSem(int semId,int semNum){
	struct sembuf sop;
	int flags;

	flags = 0;
	if(bsUseSemUndo){
		flags |= SEM_UNDO;
	}
	sop.sem_num = semNum;
	sop.sem_flg = flags;
	sop.sem_op = 1;

	return semop(semId,&sop,1); 
}
