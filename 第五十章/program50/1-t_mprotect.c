#define _BSD_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

#define LEN (4096 * 10)

#define SHELL_FMT "cat /proc/%ld/maps | grep zero"
#define CMD_SIZE (sizeof(SHELL_FMT) + 20)

int main(int argc,char *argv[]){
	char *addr;
	char cmd[CMD_SIZE];

	addr = mmap(NULL,LEN,PROT_NONE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	if(addr == MAP_FAILED){
		fprintf(stderr,"mmap\n");
		exit(EXIT_FAILURE);
	}

	printf("before mprotcet:\n");
	snprintf(cmd,CMD_SIZE,SHELL_FMT,(long)getpid());
	system(cmd);

	if(mprotect(addr,4096 * 2,PROT_READ | PROT_WRITE) == -1){
		fprintf(stderr,"mprotect\n");
		exit(EXIT_FAILURE);
	}

	printf("after mprotcet:\n");
	system(cmd);

	return 0;
}
