#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char *argv[]){
	int fd;

	fd = open("4-xxx",O_CREAT | O_TRUNC | O_WRONLY,(mode_t)0);
	if(fd == -1){
		if(errno == EACCES){
			printf("other process locked\n");
			exit(EXIT_FAILURE);
		}
	}

	printf("got lock\n");

	return 0;
}
