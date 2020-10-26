#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>

int main(int argc,char *argv[]){
	int i,pol;
	struct sched_param sp;

	for(i = 1; i < argc; i++){
		pol = sched_getscheduler(atol(argv[i]));
		if(pol == -1){
			fprintf(stderr,"gets\n");
			exit(EXIT_FAILURE);
		}

		if(sched_getparam(atol(argv[i]),&sp) == -1){
			fprintf(stderr,"getp\n");
			exit(EXIT_FAILURE);
		}

		printf("%s: %-5s %2d\n",argv[i],
				(pol == SCHED_OTHER) ? "OTHER" :
				(pol == SCHED_FIFO) ? "FIFO" :
				(pol == SCHED_RR) ? "RR" :
#ifdef SCHED_BATCH
				(pol == SCHED_BATCH) ? "BATCH" :
#endif
#ifdef SCHED_IDLE
				(pol == SCHED_IDLE) ? "IDLE" :
#endif
				"???",sp.sched_priority
				);
	}

	return 0;
}
