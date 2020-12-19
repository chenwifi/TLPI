#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

static void displayMincore(char *addr,int len){
	long num,pageSize,i;
	unsigned char *vec;

	pageSize = sysconf(_SC_PAGESIZE);
	if(pageSize == -1){
		fprintf(stderr,"sysconf\n");
		exit(EXIT_FAILURE);
	}

	num = (len + pageSize - 1) / pageSize;
	vec = (char *)malloc(num);
	if(vec == NULL){
		fprintf(stderr,"malloc\n");
		exit(EXIT_FAILURE);
	}
	if(mincore(addr,len,vec) == -1){
		fprintf(stderr,"mincore\n");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < num; i++){
		if(i % 64 == 0){
			printf("%s%10p: ",(i == 0) ? "" : "\n",addr + i * pageSize);
		}
		printf("%c",(vec[i] & 1) ? '*':'.');
	}

	printf("\n");
}

int main(int argc,char *argv[]){
	char *addr;
	long pageSize;
	int len,stepSize,lockLen,i;

	if(argc != 4 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s num-pages lock-page-step lock-page-len\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	pageSize = sysconf(_SC_PAGESIZE);
	if(pageSize == -1){
		fprintf(stderr,"sysconf\n");
		exit(EXIT_FAILURE);
	}

	len = atoi(argv[1]) * pageSize;
	stepSize = atoi(argv[2]) * pageSize;
	lockLen = atoi(argv[3]) * pageSize;

	addr = mmap(NULL,len,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	if(addr == MAP_FAILED){
		fprintf(stderr,"mmap\n");
		exit(EXIT_FAILURE);
	}

	printf("Allocated %d (%#lx) bytes starting at %p\n",(long)len,(long)len,addr);
	printf("Before mlock:\n");
	displayMincore(addr,len);

	for(i = 0; i + lockLen < len;i += stepSize){
		if(mlock(addr + i,lockLen)){
			fprintf(stderr,"mlock\n");
			exit(EXIT_FAILURE);
		}
	}

	printf("After mlock:\n");
	displayMincore(addr,len);

	return 0;
}
