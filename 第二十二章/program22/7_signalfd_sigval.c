#include <stdio.h>
#include <sys/signalfd.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc,char *argv[]){
	sigset_t set;
	int i,fd,n;
	struct signalfd_siginfo sfd;

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s [signum...]",argv[0]);
		exit(EXIT_SUCCESS);
	}

	sigemptyset(&set);
	for(i = 1; i < argc; i++){
		sigaddset(&set,atoi(argv[i]));
	}
	sigprocmask(SIG_BLOCK,&set,NULL);

	fd = signalfd(-1,&set,0);
	if(fd == -1){
		fprintf(stderr,"signalfd\n");
		exit(EXIT_SUCCESS);
	}

	while(1){
		n = read(fd,&sfd,sizeof(struct signalfd_siginfo));
		if(n != sizeof(struct signalfd_siginfo)){
			fprintf(stderr,"read\n");
			exit(EXIT_SUCCESS);
		}

		printf("got signal : %d\n",sfd.ssi_signo);
	}

	return 0;
}
