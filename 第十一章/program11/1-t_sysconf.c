#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static void sysconfPrint(const char *msg,int name){
	int lim;

	errno = 0;

	lim = sysconf(name);
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
	sysconfPrint("_SC_ARG_MAX:  ",_SC_ARG_MAX);

	return 0;
}
