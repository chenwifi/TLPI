#ifdef USE_MAP_ANON
#define _BSD_SOURCE
#endif
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>

int main(int argc,char *argv[]){
	char *addr;

#ifdef USE_MAP_ANON
	addr = mmap(NULL,sizeof(int),PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	if(addr == MAP_FAILED){
		fprintf(stderr,"mmap\n");
		exit(EXIT_FAILURE);
	}
	printf("use map anon\n");
#else
	int fd;

	fd = open("/dev/zero",O_RDWR);
	if(fd == -1){
		fprintf(stderr,"open\n");
		exit(EXIT_FAILURE);
	}

	addr = mmap(NULL,sizeof(int),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if(addr == MAP_FAILED){
		fprintf(stderr,"mmap\n");
		exit(EXIT_FAILURE);
	}

	close(fd);

	printf("use dev zero\n");
#endif

	*addr = 1;

	switch(fork()){
		case -1:
			fprintf(stderr,"fork\n");
			exit(EXIT_FAILURE);
			break;
		case 0:
			printf("Child started,value = %d\n",*addr);
			(*addr)++;

			if(munmap(addr,sizeof(int)) == -1){
				fprintf(stderr,"munmap\n");
				exit(EXIT_FAILURE);
			}
			break;
		default:
			if(wait(NULL) == -1){
				fprintf(stderr,"wait\n");
				exit(EXIT_FAILURE);
			}
			printf("in parent value = %d\n",*addr);
			if(munmap(addr,sizeof(int)) == -1){
				fprintf(stderr,"munmap\n");
				exit(EXIT_FAILURE);
			}
			break;
	}

	return 0;
}
