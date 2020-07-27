#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>

static void handler(int sig){
	return ;
}

int main(int argc,char *argv[]){
	struct timespec req,rem;
	struct timeval start,finish;
	struct sigaction sa;
	int s;

	if(argc != 3 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"usage: %s secs nanosecs\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	req.tv_sec = atol(argv[1]);
	req.tv_nsec = atol(argv[1]);

	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(SIGINT,&sa,NULL);

	gettimeofday(&start,NULL);
	while(1){
		s = nanosleep(&req,&rem);
		if(s == -1 && errno != EINTR){
			fprintf(stderr,"nanosleep err\n");
			exit(EXIT_FAILURE);
		}

		gettimeofday(&finish,NULL);
		printf("sleep for: %9.6f secs\n",finish.tv_sec - start.tv_sec + (finish.tv_usec - start.tv_usec) / 1000000.0);
		
		if(s == 0){
			break;
		}

		printf("remain: %2ld.%09ld\n",(long)rem.tv_sec,(long)rem.tv_nsec);

		req = rem;
	}

	return 0;
}
