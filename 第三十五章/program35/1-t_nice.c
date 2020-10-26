#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc,char *argv[]){
	int which,prio,who;
	int tmp;

	if(argc != 4 || strchr("pgu",argv[1][0]) == NULL){
		fprintf(stderr,"Usage: %s {p|g|s} who prio\n");
		exit(EXIT_FAILURE);
	}

	which = (argv[1][0] == 'p') ? PRIO_PROCESS : (argv[1][0] == 'g') ? PRIO_PGRP : PRIO_USER;
	who = atoi(argv[2]);
	prio = atoi(argv[3]);

	errno = 0;
	tmp = getpriority(which,who);
	if(tmp == -1 && errno != 0){
		fprintf(stderr,"getp\n");
		exit(EXIT_FAILURE);
	}
	printf("the default prio is %d\n",tmp);

	if(setpriority(which,who,prio) == -1){
		fprintf(stderr,"setpriority\n");
		exit(EXIT_FAILURE);
	}

	errno = 0;
	tmp = getpriority(which,who);
	if(tmp == -1 && errno != 0){
		fprintf(stderr,"getp\n");
		exit(EXIT_FAILURE);
	}
	printf("the current prio is %d\n",tmp);

	return 0;
}
