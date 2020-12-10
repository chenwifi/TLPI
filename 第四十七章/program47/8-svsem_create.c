#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	int opt,flags,numSems,semid;
	unsigned int perms;
	int numKeyFlags;
	key_t key,lkey;

	flags = 0;
	numKeyFlags = 0;
	while((opt = getopt(argc,argv,"cf:k:px")) != -1){
		switch(opt){
			case 'c':
				flags |= IPC_CREAT;
				break;
			case 'f':
				key = ftok(optarg,1);
				if(key == -1){
					fprintf(stderr,"ftok\n");
					exit(EXIT_FAILURE);
				}
				numKeyFlags++;
				break;
			case 'k':
				if(sscanf(optarg,"%li",&lkey) != 1){
					fprintf(stderr,"sscanf\n");
					exit(EXIT_FAILURE);
				}
				key = lkey;
				numKeyFlags++;
				break;
			case 'p':
				key = IPC_PRIVATE;
				numKeyFlags++;
				break;
			case 'x':
				flags |= IPC_EXCL;
				break;
			default:
				exit(EXIT_FAILURE);
				break;
		}
	}

	if(numKeyFlags != 1){
		fprintf(stderr,"only one option for key\n");
		exit(EXIT_FAILURE);
	}

	if(optind >= argc){
		fprintf(stderr,"no nsem\n");
		exit(EXIT_FAILURE);
	}

	numSems = atoi(argv[optind]);
	perms = (optind + 1 >= argc) ? (S_IRUSR | S_IWUSR) : strtol(argv[optind + 1],NULL,8); 

	semid = semget(key,numSems,perms | flags);
	if(semid == -1){
		fprintf(stderr,"semget\n");
		exit(EXIT_FAILURE);
	}

	printf("%d\n",semid);

	return 0;
}
