#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>

int main(int argc,char *argv[]){

	if(argc != 2 || strcmp(argv[1],"--help") == 0 ){
		fprintf(stderr,"Usage: %s sem-name\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	if(sem_unlink(argv[1]) != 0){
		fprintf(stderr,"sem_unlink\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
