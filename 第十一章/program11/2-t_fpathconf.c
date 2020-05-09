#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static void fpathconfPrint(const char *msg,int fd,int name){
	int lim;

	errno = 0;

	lim = fpathconf(fd,name);
	if(lim != -1){
		printf("%s %ld\n",msg,lim);
	}else{
		if(errno != 0){
			exit(EXIT_FAILURE);
		}else{
			printf("%s (indeterminate)\n",msg);
		}
	}
}

int main(int argc,char *argv[]){
	fpathconfPrint("_PC_NAME_MAX: ",STDIN_FILENO,_PC_NAME_MAX);

	return 0;
}
