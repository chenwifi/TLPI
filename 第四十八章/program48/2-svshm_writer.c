#include "1-svshm.h"
#include "semun.h"
#include "binary_sems.h"

int main(int argc,char *argv[]){
	int semid,shmid;
	struct shmseg *shm;
	int len,i;
	union semun arg;

	semid = semget(SEM_KEY,2,IPC_CREAT | OBJ_PERMS);
	if(semid == -1){
		fprintf(stderr,"semget,%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	shmid = shmget(SHM_KEY,sizeof(struct shmseg),IPC_CREAT | OBJ_PERMS);
	if(shmid == -1){
		fprintf(stderr,"shmid\n");
		exit(EXIT_FAILURE);
	}

	if(initSemAvailable(semid,WRITE_SEM) == -1){
		fprintf(stderr,"initSem\n");
		exit(EXIT_FAILURE);
	}

	if(initSemInUse(semid,READ_SEM) == -1){
		fprintf(stderr,"initSem\n");
		exit(EXIT_FAILURE);
	}

	shm = shmat(shmid,NULL,0);
	if(shm == NULL){
		fprintf(stderr,"shmat\n");
		exit(EXIT_FAILURE);
	}

	for(len = 0,i = 0;;len += shm->cnt,i++){
		if(reserveSem(semid,WRITE_SEM) == -1){
			fprintf(stderr,"reserveSem\n");
			exit(EXIT_FAILURE);
		}

		shm->cnt = read(STDIN_FILENO,shm->buf,BUF_SIZE);
		if(shm->cnt == -1){
			fprintf(stderr,"read\n");
			exit(EXIT_FAILURE);
		}

		if(releaseSem(semid,READ_SEM) == -1){
			fprintf(stderr,"read\n");
			exit(EXIT_FAILURE);
		}

		if(shm->cnt == 0){
			break;
		}
	}

	if(reserveSem(semid,WRITE_SEM) == -1){
		fprintf(stderr,"reserveSem\n");
		exit(EXIT_FAILURE);
	}

	if(semctl(semid,0,IPC_RMID,arg) == -1){
		fprintf(stderr,"semctl\n");
		exit(EXIT_FAILURE);
	}

	if(shmdt(shm) == -1){
		fprintf(stderr,"shmdt\n");
		exit(EXIT_FAILURE);
	}

	if(shmctl(shmid,IPC_RMID,NULL) == -1){
		fprintf(stderr,"semctl\n");
		exit(EXIT_FAILURE);
	}

	fprintf(stderr,"Sent %d bytes (%d cnt)\n",len,i);

	return 0;
}
