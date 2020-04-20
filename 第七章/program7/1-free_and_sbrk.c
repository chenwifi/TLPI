#include "tlpi_hdr.h"

#define MAX_ALLOCS 1000000

int main(int argc,char *argv[]){
	char *buf[MAX_ALLOCS];
	int mnum,msize,freeStep,freeMin,freeMax,i;

	if(argc < 3 || strcmp(argv[1],"--help") == 0){
		usageErr("%s memnum memsize",argv[0]);
	}

	mnum = getInt(argv[1],GN_GT_0,"mnum");
	if(mnum > MAX_ALLOCS){
		errExit("mnum error");
	}
	msize = getInt(argv[2],GN_GT_0,"msize");
	freeStep = argc > 3 ? getInt(argv[3],GN_GT_0,"freeStep") : 1;
	freeMin = argc > 4 ? getInt(argv[4],GN_GT_0,"freeMin") : 1;
	freeMax = argc > 5 ? getInt(argv[5],GN_GT_0,"freeMax") : mnum;
	if(freeMax > mnum){
		cmdLineErr("free max > mnum");
	}

	printf("Initial program break: %p\n",sbrk(0));
	printf("Allocating %d * %d bytes\n",mnum,msize);

	for(i = 0;i < mnum;i++){
		buf[i] = (char *)malloc(msize);
		if(buf[i] == NULL){
			errExit("malloc");
		}
	}
	printf("program break is now: %p\n",sbrk(0));

	printf("Freeing blocks from %d to %d\n",freeMin,freeMax);
	for(i = freeMin - 1;i < freeMax ; i = i + freeStep){
		free(buf[i]);
	}

	printf("After free(),program break is: %p\n",sbrk(0));

	return 0;
}
