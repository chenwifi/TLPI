#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>

//the right way is open(filename,O_CREAT|O_EXCL,0666)
//this pro has a bug.
int main(int argc,char *argv[]){
	int fd;

	fd = open(argv[1],O_WRONLY,0666);

	if(fd != -1){
		printf("[PID %ld] File \"%s\" already exist\n",getpid(),argv[1]);
	}else{
		if(errno != ENOENT){
			errExit("open");
		}else{
			if(argc > 2){
				printf("sleep for seconds\n");
				sleep(5);
			}
			fd = open(argv[1],O_WRONLY | O_CREAT,0666);
			if(fd == -1){
				errExit("open");
			}

			printf("[PID %ld] File \"%s\" create file exclusivly\n",getpid(),argv[1]);
		}
	}
}
