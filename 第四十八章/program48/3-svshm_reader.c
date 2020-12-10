#include "1-svshm.h"
#include "binary_sems.h"

int main(int argc,char *argv[]){
	int semid,shmid;
	struct shmseg *shm;
	int i,bytes;
	ssize_t len;

	semid = semget(SEM_KEY,2,0);
	if(semid == -1){
		fprintf(stderr,"semget\n");
		exit(EXIT_FAILURE);
	}

	shmid = shmget(SHM_KEY,0,0);
	if(shmid == -1){
		fprintf(stderr,"shmget\n");
		exit(EXIT_FAILURE);
	}

	shm = shmat(shmid,NULL,SHM_RDONLY);
	if(shm == NULL){
		fprintf(stderr,"shmat\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0,bytes = 0;;i++){
		if(reserveSem(semid,READ_SEM) == -1){
			fprintf(stderr,"reserveSem\n");
			exit(EXIT_FAILURE);
		}

		if(shm->cnt == 0){
			break;
		}

		bytes += shm->cnt;
		len = write(STDOUT_FILENO,shm->buf,shm->cnt);
		if(len != shm->cnt){
			fprintf(stderr,"write\n");
			exit(EXIT_FAILURE);
		}


		if(releaseSem(semid,WRITE_SEM) == -1){
			fprintf(stderr,"reserveSem\n");
			exit(EXIT_FAILURE);
		}
	}

	if(shmdt(shm) == -1){
		fprintf(stderr,"reserveSem\n");
		exit(EXIT_FAILURE);
	}

	if(releaseSem(semid,WRITE_SEM) == -1){
		fprintf(stderr,"reserveSem\n");
		exit(EXIT_FAILURE);
	}

	fprintf(stderr,"Received %d bytes (%d cnt)\n",bytes,i);

	return 0;
}
