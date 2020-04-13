#include "tlpi_hdr.h" 
#include <fcntl.h>
#include <sys/stat.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc,char *argv[]){
	int inputFd,outputFd,openFlags;
	mode_t filePerms;
	ssize_t numRead;
	char buf[BUF_SIZE];

	if(strcmp(argv[1],"--help") == 0 || argc != 3){
		usageErr("%s old-file new-file\n",argv[0]);
	}

	if((inputFd = open(argv[1],O_RDONLY)) == -1){
		errExit("opening file %s",argv[1]);
	}

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = 0666;

	outputFd = open(argv[2],openFlags,filePerms);
	if(outputFd == -1)
		errExit("opening file %s",argv[2]);

	while((numRead = read(inputFd,buf,BUF_SIZE)) > 0){
		if(write(outputFd,buf,numRead) != numRead){
			fatal("could not write whole buffer");
		}
	}

	if(numRead == -1){
		errExit("read");
	}

	if(close(inputFd) == -1){
		errExit("close input");
	}

	if(close(outputFd) == -1){
		errExit("close output");
	}

	exit(EXIT_SUCCESS);
}
