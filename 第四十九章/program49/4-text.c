#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>

int main(int argc,char *argv[]){
	char *addr;
	char *tmp;
	int fd;

	addr = mmap(NULL,1024,PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
	if(addr == MAP_FAILED){
		exit(EXIT_FAILURE);
	}

	memset(addr,0,10);
	strcpy(addr,"bigfat");

	printf("addr:%p\n",addr);

	fd = open("./s.txt",O_RDWR);
	if(fd == -1){
		exit(EXIT_FAILURE);
	}

	tmp = mmap(addr,1024,PROT_READ, MAP_PRIVATE | MAP_SHARED | MAP_FIXED,fd,0);
	if(tmp == MAP_FAILED){
		exit(EXIT_FAILURE);
	}

	printf("%d\n",sysconf(_SC_PAGESIZE));
	printf("tmp:%p\n",tmp);
	printf("addr:%s\n",addr);
	printf("tmp:%s\n",tmp);

	return 0;
}
