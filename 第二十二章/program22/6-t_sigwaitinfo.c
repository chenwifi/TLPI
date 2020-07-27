#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int main(int argc,char *argv[]){
	sigset_t allSet;
	siginfo_t si;
	int sig;

	if(argc > 1 && strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage : %s [delay_sec]\n",argv[0]);
		exit(EXIT_SUCCESS);
	}

	printf("%s : PID is %ld\n",argv[0],(long)getpid());

	sigfillset(&allSet);
	sigprocmask(SIG_SETMASK,&allSet,NULL);

	if(argc > 1){
		printf("%s : about to delay %s sec\n",argv[0],argv[1]);
		sleep(atoi(argv[1]));
		printf("%s : delay ending\n",argv[0]);
	}

	while(1){
		sig = sigwaitinfo(&allSet,&si);

		if(sig == SIGINT || sig == SIGTERM){
			exit(EXIT_SUCCESS);
		}

	printf("	si_signo=%d,si_code=%d (%s)",si.si_signo,si.si_code,(si.si_code == SI_USER) ? "SI_USER" : (si.si_code == SI_QUEUE) ? "SI_QUEUE" : "other");
	printf("si_value=%d",si.si_value.sival_int);
	printf("si_pid=%ld,uid=%ld\n",(long)si.si_pid,(long)si.si_uid);
	}

	return 0;
}
