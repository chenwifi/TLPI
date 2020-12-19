#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	int fd;
	struct stat buf;
	char *addr;

	if(argc != 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s file\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1],O_RDONLY);
	if(fd == -1){
		fprintf(stderr,"open\n");
		exit(EXIT_FAILURE);
	}

	if(fstat(fd,&buf) == -1){
		fprintf(stderr,"fstat\n");
		exit(EXIT_FAILURE);
	}

	addr = mmap(NULL,buf.st_size,PROT_READ,MAP_PRIVATE,fd,0);
	if(addr == MAP_FAILED){
		fprintf(stderr,"mmap\n");
		exit(EXIT_FAILURE);
	}

	//sleep(20);

	if(write(STDOUT_FILENO,addr,buf.st_size) != buf.st_size){
		fprintf(stderr,"write\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
