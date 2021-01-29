#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/file.h>

int main(int argc,char *argv[]){
	int fd,lock;
	char *lname;

	if(argc < 3 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s file lock [sleep]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	lock = (argv[2][0] == 's') ? LOCK_SH : LOCK_EX;
	if(argv[2][1] == 'n'){
		lock |= LOCK_NB;
	}

	fd = open(argv[1],O_RDONLY);
	if(fd == -1){
		fprintf(stderr,"open\n");
		exit(EXIT_FAILURE);
	}

	lname = (lock & LOCK_SH) ? "LOCK_SH" : "LOCK_EX";
	printf("PID %ld : requesting %s\n",(long)getpid(),lname);

	if(flock(fd,lock) == -1){
		if(errno == EWOULDBLOCK){
			printf("PID %ld : already locked - byte!",(long)getpid());
		}else{
			fprintf(stderr,"flock\n");
		}

		exit(EXIT_FAILURE);
	}

	printf("PID %ld : granted %s\n",(long)getpid(),lname);

	sleep((argc > 3) ? atoi(argv[3]) : 10);

	printf("PID %ld : releasing %s\n",(long)getpid(),lname);

	if(flock(fd,LOCK_UN) == -1){
		fprintf(stderr,"flock\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
