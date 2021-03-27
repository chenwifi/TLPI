#include <stdio.h>
#include <poll.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]){
	int numpipes,numwrites,i,nwrite,ret;
	int (*pipefdp)[2];
	struct pollfd *fds;
	
	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s num-pipes [num-writes]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	numpipes = atoi(argv[1]);
	numwrites = argc > 2 ? atoi(argv[2]) : 1;
	pipefdp = calloc(numpipes,sizeof(int [2]));
	fds = calloc(numpipes,sizeof(struct pollfd));

	for(i = 0; i < numpipes; i++){
		if(pipe(pipefdp[i]) == -1){
			fprintf(stderr,"pipe\n");
			exit(EXIT_FAILURE);
		}
	}

	srand(time(NULL));
	for(i = 0; i < numwrites; i++){
		nwrite = rand() % numpipes;
		printf("write to the %d pipe\n",nwrite);
		if(write(pipefdp[nwrite][1],"a",1) != 1){
			fprintf(stderr,"write\n");
			exit(EXIT_FAILURE);
		}
	}

	for(i = 0; i < numpipes; i++){
		fds[i].fd = pipefdp[i][0];
		fds[i].events = POLLIN;
	}

	ret = poll(fds,numpipes,-1);

	if(ret < 0){
		fprintf(stderr,"poll\n");
		exit(EXIT_FAILURE);
	}

	printf("return %d\n",ret);

	for(i = 0; i < numpipes; i++){
		if(fds[i].revents & POLLIN){
			printf("raeadable : %d\n",i);
		}
	}

	return 0;
}
