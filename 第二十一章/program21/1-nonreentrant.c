#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

static char *str2;
static int handled = 0;

static void handler(int sig){
	crypt(str2,"xx");
	handled++;
}

int main(int argc,char *argv[]){
	char *cr1;
	struct sigaction a;

	if(argc != 3){
		fprintf(stderr,"Usage: %s str1 str2\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	str2 = argv[2];
	cr1 = strdup(crypt(argv[1],"xx"));

	sigemptyset(&a.sa_mask);
	a.sa_handler = handler;
	a.sa_flags = 0;
	sigaction(SIGINT,&a,NULL);

	while(1){
		if(strcmp(cr1,crypt(argv[1],"xx")) != 0){
			printf("mismatch\n");
		}
	}

	return 0;
}
