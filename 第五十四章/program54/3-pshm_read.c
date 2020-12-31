#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char *argv[]){
	int fd;
	struct stat buf;
	char *addr;

	if(argc != 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s shm-name\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = shm_open(argv[1],O_RDWR,0);
	if(fd == -1){
		fprintf(stderr,"shm_open\n");
		exit(EXIT_FAILURE);
	}

	if(fstat(fd,&buf) == -1){
		fprintf(stderr,"fstat\n");
		exit(EXIT_FAILURE);
	}

	addr = mmap(0,buf.st_size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if(addr == MAP_FAILED){
		fprintf(stderr,"mmap\n");
		exit(EXIT_FAILURE);
	}

	if(close(fd) == -1){
		fprintf(stderr,"close\n");
		exit(EXIT_FAILURE);
	}

	write(STDOUT_FILENO,addr,buf.st_size);
	printf("\n");

	return 0;
}
