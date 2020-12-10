#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	int numKeyFlags;
	int opt,flags,msqid;
	key_t key;
	unsigned int perms;
	long lkey;
	char *endptr;

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
			default :
				fprintf(stderr,"Bad option\n");
				exit(EXIT_FAILURE);
				break;
		}
	}

	if(numKeyFlags != 1){
		fprintf(stderr,"Only one option must be supplied\n");
		exit(EXIT_FAILURE);
	}

	perms = (optind == argc) ? (S_IRUSR | S_IWUSR) : (int)strtol(argv[optind],&endptr,8); 
	msqid = msgget(key,flags | perms);
	if(msqid == -1){
		fprintf(stderr,"msgget\n");
		exit(EXIT_FAILURE);
	}

	printf("%d\n",msqid);

	return 0;
}
