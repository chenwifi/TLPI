#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "1-become_daemon.h"

int main(){

	if(becomeDaemon(0) == -1){
		fprintf(stderr,"becomeDaemon error\n");
		exit(EXIT_FAILURE);
	}

	sleep(30);

	return 0;
}
