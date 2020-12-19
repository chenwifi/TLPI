#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 10

int main(int argc,char *argv[]){
	int fd;
	char *addr;

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s file [new-value]\n");
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1],O_RDWR);
	if(fd == -1){
		fprintf(stderr,"open\n");
		exit(EXIT_FAILURE);
	}

	addr = mmap(NULL,MEM_SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if(addr == MAP_FAILED){
		fprintf(stderr,"mmap\n");
		exit(EXIT_FAILURE);
	}

	close(fd);

	printf("Current string = %.*s\n",MEM_SIZE,addr);

	if(argc > 2){
		if(strlen(argv[1]) >= MEM_SIZE){
			fprintf(stderr,"string too long\n");
			exit(EXIT_FAILURE);
		}

		memset(addr,0,MEM_SIZE);
		strncpy(addr,argv[2],MEM_SIZE - 1);

		printf("copy %s to shared mem\n",argv[2]);
	}

	return 0;
}
