#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
	int opt,flags;
	mode_t perms;
	unsigned int value;
	sem_t *sem;

	flags = 0;
	while((opt = getopt(argc,argv,"cx")) != -1){
		switch(opt){
			case 'c':
				flags |= O_CREAT;
				break;
			case 'x':
				flags |= O_EXCL;
				break;
			default:
				exit(EXIT_FAILURE);
				break;
		}
	}

	if(optind >= argc){
		exit(EXIT_FAILURE);
	}

	perms = (argc <= optind + 1) ? (S_IRUSR | S_IWUSR) : strtol(argv[optind + 1],NULL,8);
	value = (argc <= optind + 2) ? 0 : atoi(argv[optind + 2]);

	sem = sem_open(argv[optind],flags,perms,value);
	if(sem == SEM_FAILED){
		fprintf(stderr,"sem_open\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
