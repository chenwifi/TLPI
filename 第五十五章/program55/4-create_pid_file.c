#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "3-region_locking.h"
#include "4-create_pid_file.h"

#define BUF_SIZE 100

int createPidFile(const char *progName,const char *pidFile,int flags){
	int fd;
	char buf[BUF_SIZE];

	fd = open(pidFile,O_RDWR | O_CREAT,S_IRUSR | S_IWUSR);
	if(fd == -1){
		fprintf(stderr,"could not open PID file: %s\n",pidFile);
		exit(EXIT_FAILURE);
	}

	if(flags & CPF_CLOEXEC){
		flags = fcntl(fd,F_GETFD);
		if(flags == -1){
			fprintf(stderr,"fcntl\n");
			exit(EXIT_FAILURE);
		}

		flags |= FD_CLOEXEC;

		if(fcntl(fd,F_SETFD,flags) == -1){
			fprintf(stderr,"fcntl\n");
			exit(EXIT_FAILURE);
		}
	}

	if(lockRegion(fd,F_WRLCK,SEEK_SET,0,0) == -1){
		if(errno == EAGAIN || errno == EACCES){
			fprintf(stderr,"PID file %s is locked ; probably %s is already running\n",pidFile,progName);
			exit(EXIT_FAILURE);
		}else{
			fprintf(stderr,"lockRegion\n");
			exit(EXIT_FAILURE);
		}
	}

	if(ftruncate(fd,0) == -1){
		fprintf(stderr,"ftruncate\n");
		exit(EXIT_FAILURE);
	}

	snprintf(buf,BUF_SIZE,"%ld\n",(long)getpid());
	if(write(fd,buf,strlen(buf)) != strlen(buf)){
		fprintf(stderr,"write\n");
		exit(EXIT_FAILURE);
	}

	return fd;
}
