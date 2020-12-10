#include <stdio.h>
#include <stdlib.h>
#include "2-semun.h"

int main(int argc,char *argv[]){
	union semun arg;
	int i;

	if(argc > 1 && strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s [semid...]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	for(i = 1; i < argc; i++){
		if(semctl(atoi(argv[i]),0,IPC_RMID,arg) == -1){
			fprintf(stderr,"semctl\n");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
