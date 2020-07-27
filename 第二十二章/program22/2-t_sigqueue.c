#define _POSIX_C_SOURCE 199309
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main(int argc,char *argv[]){
	int sig,numSigs,sigData,i;
	union sigval sv;

	if(argc < 4 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s pid sig data <times>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	printf("%s: PID is %ld, UID is %ld\n",argv[0],(long)getpid(),(long)getuid());

	sig = atoi(argv[2]);
	sigData = atoi(argv[3]);
	numSigs = (argc > 4) ? atoi(argv[4]) : 1;

	for(i = 0;i < numSigs;i++){
		sv.sival_int = sigData + i;
		sigqueue(atol(argv[1]),sig,sv);
	}

	return 0;
}
