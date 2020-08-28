#include <stdio.h>
#include <unistd.h>
#include <libgen.h>

int main(int argc,char *argv[]){
	char *envVec[] = {"GREET=shabi","BYE=dashabi",NULL};
	char *filename;

	filename = basename(argv[1]);
	printf("%s\n",filename);

	execle(argv[1],filename,"hello world",(char *)NULL,envVec);

	return 0;
}
