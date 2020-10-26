#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	pid_t a;
	int res;

	a = (pid_t)atol(argv[1]);

	switch(fork()){
		case -1:
			break;
		case 0:
			sleep(10);
			break;
		default:
			res = setpgid(0,a);
			while(res == 0){
				printf("success\n");
			}
			break;
	}

	return 0;
}
