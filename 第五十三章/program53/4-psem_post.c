#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>

int main(int argc,char *argv[]){
	sem_t *sem;

	if(argc != 2 || strcmp(argv[1],"--help") == 0 ){
		fprintf(stderr,"Usage: %s sem-name\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	sem = sem_open(argv[1],0);
	if(sem == SEM_FAILED){
		fprintf(stderr,"sem_open\n");
		exit(EXIT_FAILURE);
	}

	if(sem_post(sem) == -1){
		fprintf(stderr,"sem_post\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
