#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	int res;

	//res = setpgid(0,0);
	if(res < 0){
		fprintf(stderr,"setpgid\n");
		exit(EXIT_FAILURE);
	}

	printf("from tpgid\n");

	sleep(20);

	return 0;
}
