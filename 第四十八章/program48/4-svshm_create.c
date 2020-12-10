#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc,char *argv[]){
	int numKeyFlags;
	int flags,opt,segSize;
	unsigned int perms;
	key_t key;
	long lkey;
	int shmid;

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
					exit(EXIT_FAILURE);
				}
				numKeyFlags++;
				break;
			case 'k':
				if(sscanf(optarg,"%li",&lkey) != 1){
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
				break;
		}
	}

	if(numKeyFlags != 1){
		exit(EXIT_FAILURE);
	}

	if(optind >= argc){
		exit(EXIT_FAILURE);
	}

	segSize = atoi(argv[optind]);
	perms = (argc <= optind + 1) ? (S_IRUSR | S_IWUSR) : strtol(argv[optind + 1],NULL,8);

	shmid = shmget(key,segSize,flags | perms);
	if(shmid == -1){
		exit(EXIT_FAILURE);
	}

	printf("%d\n",shmid);

	return 0;
}
