#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main(int argc,char *argv[]){
	int policy,i;
	struct sched_param sp;

	policy = (argv[1][0] == 'r') ? SCHED_RR : (argv[1][0] == 'f') ? SCHED_FIFO : 
#ifdef SCHED_BATCH
		(argv[1][0] == 'b') ? SCHED_BATCH : 
#endif
#ifdef SCHED_IDLE 
		(argv[1][0] == 'i') ? SCHED_IDLE : 
#endif
		SCHED_OTHER;

	sp.sched_priority = atoi(argv[2]);

	for(i = 3; i < argc; i++){
		if((sched_setscheduler(atol(argv[i]),policy,&sp)) == -1){
			fprintf(stderr,"set\n");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
