#include "tlpi_hdr.h"

int main(){
	char filename[] = "bigfatXXXXXX";
	int fd;

	fd = mkstemp(filename);
	if(fd == -1){
		errExit("mkstemp");
	}

	printf("create file named %s\n",filename);
	sleep(5);

	unlink(filename);

	if(close(fd) == -1){
		errExit("close");
	}

	return 0;
}
