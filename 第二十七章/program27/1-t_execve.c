#include <stdio.h>
#include <libgen.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	char *argVec[10];
	char *envVec[] = {"GREET=shabi","BYE=dashabi",NULL};

	argVec[0] = basename(argv[1]);
	argVec[1] = "hello world";
	argVec[2] = "nono";
	argVec[3] = NULL;

	execve(argv[1],argVec,envVec);

	return 0;
}
