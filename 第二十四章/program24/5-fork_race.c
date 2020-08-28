#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	int i,numChild;
	pid_t childPid;

	if(argc > 1 && strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"usage: %s numchild\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	numChild = (argc > 1) ? atoi(argv[1]) : 1;

	for(i = 0; i < numChild; i++){
		switch(fork()){
			case -1:
				break;
			case 0:
				printf("%d child\n",i);
				_exit(EXIT_FAILURE);
				break;
			default:
				printf("%d parent\n",i);
				wait(NULL);
				break;
		}
	}

	return 0;
}
