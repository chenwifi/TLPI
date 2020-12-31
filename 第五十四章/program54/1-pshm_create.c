#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc,char *argv[]){
	int flags,opt,fd;
	size_t size;
	mode_t perms;
	void *addr;

	flags = O_RDWR;
	while((opt = getopt(argc,argv,"cx")) != -1){
		switch(opt){
			case 'c':
				flags |= O_CREAT;
				break;
			case 'x':
				flags |= O_EXCL;
				break;
			default:
				exit(EXIT_FAILURE);
				break;
		}
	}

	if(optind + 1 >= argc){
		exit(EXIT_FAILURE);
	}

	size = atoi(argv[optind + 1]);
	perms = (argc <= optind + 2) ? (S_IRUSR | S_IWUSR) : strtol(argv[optind + 2],NULL,8);

	fd = shm_open(argv[optind],flags,perms);
	if(fd == -1){
		fprintf(stderr,"shm_open\n");
		exit(EXIT_FAILURE);
	}

	if(ftruncate(fd,size) == -1){
		fprintf(stderr,"ftruncate\n");
		exit(EXIT_FAILURE);
	}

	addr = mmap(NULL,size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if(addr == MAP_FAILED){
		fprintf(stderr,"mmap:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	return 0;
}
