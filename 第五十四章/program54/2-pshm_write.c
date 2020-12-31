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
	size_t len;
	char *addr;

	if(argc != 3 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s shm-name string\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = shm_open(argv[1],O_RDWR,0);
	if(fd == -1){
		fprintf(stderr,"shm_open\n");
		exit(EXIT_FAILURE);
	}

	len = strlen(argv[2]);
	if(ftruncate(fd,len) == -1){
		fprintf(stderr,"ftruncate\n");
		exit(EXIT_FAILURE);
	}
	printf("Resize to %d bytes\n",len);

	addr = mmap(0,len,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if(addr == MAP_FAILED){
		fprintf(stderr,"mmap\n");
		exit(EXIT_FAILURE);
	}

	if(close(fd) == -1){
		fprintf(stderr,"close\n");
		exit(EXIT_FAILURE);
	}

	printf("copying %d bytes\n",len);

	memcpy(addr,argv[2],len);

	return 0;
}
