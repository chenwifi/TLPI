#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	int pfd[2];
	int i;
	int tmp;

	setbuf(stdout,NULL);

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s sleep-time...\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	if(pipe(pfd) == -1){
		fprintf(stderr,"pipe\n");
		exit(EXIT_FAILURE);
	}

	for(i = 1; i < argc; i++){
		switch(fork()){
			case -1:
				fprintf(stderr,"fork\n");
				_exit(EXIT_FAILURE);
				break;
			case 0:
				close(pfd[0]);
				sleep(atoi(argv[i]));
				printf("child %d (%ld)  closing pipe\n",i,(long)getpid());
				close(pfd[1]);
				_exit(EXIT_SUCCESS);
				break;
			default:
				break;
		}
	}

	close(pfd[1]);

	if(read(pfd[0],&tmp,1) != 0){
		fprintf(stderr,"parent does not get EOF\n");
	}

	printf("parent ready to go\n");

	exit(EXIT_SUCCESS);

	return 0;
}
