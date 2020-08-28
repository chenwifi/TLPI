#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	printf("Hello World\n");
	write(STDOUT_FILENO,"PHP\n",4);

	switch(fork()){
		case -1:
			fprintf(stderr,"fork error\n");
			exit(EXIT_FAILURE);
		case 0:
			execlp("echo","echo","echo",(char *)NULL);
			_exit(EXIT_FAILURE);
			break;
		default:
			break;
	}

	exit(EXIT_SUCCESS);
}
