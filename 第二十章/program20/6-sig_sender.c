#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc,char *argv[]){
	int i,num,signum;
	pid_t pid;

	if(argc < 4 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s PID num signum <signum2>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	pid = atol(argv[1]);
	num = atoi(argv[2]);
	signum = atoi(argv[3]);

	printf("%s: sending signal %d to process %ld %d times\n",argv[0],signum,pid,num);

	for(i = 0;i < num;i++){
		kill(pid,signum);
	}

	if(argc > 4){
		kill(pid,atoi(argv[4]));
	}

	printf("%s: existing\n",argv[0]);

	return 0;
}
