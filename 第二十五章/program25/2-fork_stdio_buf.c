#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	printf("Hello World\n");
	write(STDOUT_FILENO,"PHP\n",4);

	fork();

	exit(EXIT_SUCCESS);
}
