#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	pid_t child;

	switch(child = fork()){
		case -1:
			break;
		case 0:
			while(1){
				pause();
			}
			break;
		default:
			kill(child,SIGSTOP);
			exit(EXIT_SUCCESS);
			break;
	}

	return 0;
}
