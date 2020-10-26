#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>

int main(int argc,char *argv[]){
	struct rlimit rl;
	int i;
	pid_t child;

	if(argc < 2 || argc > 3 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s soft-limit [hard-limit]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	printRlimit("Initial max process limits: ",RLIMIT_NPROC);

	rl.rlim_cur = (argv[1][0] == 'i') ? RLIM_INFINITY : atoi(argv[1]);
	rl.rlim_max = (argc == 2) ? rl.rlim_cur : (argv[2][0] == 'i') ? RLIM_INFINITY : atoi(argv[2]);
	if(setrlimit(RLIMIT_NPROC,&rl) == -1){
		fprintf(stderr,"setrlimit\n");
		exit(EXIT_FAILURE);
	}

	printRlimit("New max process limits: ",RLIMIT_NPROC);

	for(i = 1; ; i++){
		switch(child = fork()){
			case -1:
				fprintf(stderr,"fork error\n");
				exit(EXIT_FAILURE);
				break;
			case 0:
				_exit(EXIT_SUCCESS);
				break;
			default:
				printf("Child %d (PID=%ld) started\n",i,(long)child);
				break;
		}
	}

	return 0;
}
