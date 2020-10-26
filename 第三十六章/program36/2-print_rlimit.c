#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include "2-print_rlimit.h"

int printRlimit(const char *msg,int resource){
	struct rlimit rl;

	if(getrlimit(resource,&rl) == -1){
		fprintf(stderr,"getrlimit\n");
		exit(EXIT_FAILURE);
	}

	printf("%s soft=",msg);
	if(rl.rlim_cur == RLIM_INFINITY){
		printf("infinity");
	}
#ifdef RLIM_SAVED_CUR
	else if(rl.rlim_cur == RLIM_SAVED_CUR){
		printf("unrepresentable");
	}
#endif
	else{
		printf("%lld",rl.rlim_cur);
	}

	printf("; hard=");
	if(rl.rlim_max == RLIM_INFINITY){
		printf("infinity\n");
	}
#ifdef RLIM_SAVED_CUR
	else if(rl.rlim_max == RLIM_SAVED_CUR){
		printf("unrepresentable\n");
	}
#endif
	else{
		printf("%lld\n",rl.rlim_max);
	}

	return 0;
}
