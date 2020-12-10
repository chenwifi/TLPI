#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "2-semun.h"

int main(int argc,char *argv[]){
	int semid,nsems,i;
	union semun arg;

	if(argc != 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s sems\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	nsems = atoi(argv[1]);

	if((semid = semget(IPC_PRIVATE,nsems,S_IRUSR | S_IWUSR)) == -1){
		fprintf(stderr,"semget\n");
		exit(EXIT_FAILURE);
	}

	arg.array = calloc(nsems,sizeof(arg.array[0]));
	if(arg.array == NULL){
		fprintf(stderr,"calloc\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < nsems; i++){
		arg.array[i] = 1;
	}

	if(semctl(semid,0,SETALL,arg) == -1){
		fprintf(stderr,"calloc\n");
		exit(EXIT_FAILURE);
	}

	printf("semid = %d\n",semid);

	return 0;
}
