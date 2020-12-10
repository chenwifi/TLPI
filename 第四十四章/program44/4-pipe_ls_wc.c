#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	int pfd[2];

	if(pipe(pfd) != 0){
		fprintf(stderr,"pipe\n");
		exit(EXIT_FAILURE);
	}

	switch(fork()){
		case -1:
			exit(EXIT_FAILURE);
			break;
		case 0:
			close(pfd[0]);
			if(pfd[1] != STDOUT_FILENO){
				dup2(pfd[1],STDOUT_FILENO);
				close(pfd[1]);
			}

			execlp("ls","ls",NULL);

			exit(EXIT_SUCCESS);
			break;
		default:
			break;
	}

	switch(fork()){
		case -1:
			exit(EXIT_FAILURE);
			break;
		case 0:
			close(pfd[1]);
			if(pfd[0] != STDIN_FILENO){
				dup2(pfd[0],STDIN_FILENO);
				close(pfd[0]);
			}

			execlp("wc","wc","-l",NULL);

			exit(EXIT_SUCCESS);
			break;
		default:
			break;
	}

	close(pfd[0]);
	close(pfd[1]);
	wait(NULL);
	wait(NULL);

	return 0;
}
