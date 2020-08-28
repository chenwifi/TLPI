#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char *argv[]){
	pid_t cpid;
	char filename[] = "templateXXXXXX";
	int fd,flags;
	off_t offset;

	/*
	*filename = 'w';

	printf("%s\n",filename);
	*/

	fd = mkstemp(filename);
	offset = lseek(fd,0,SEEK_CUR);
	printf("FILE offset before fork(): %lld\n",(long long)offset);

	flags = fcntl(fd,F_GETFL);
	printf("O_APPEND flag before fork() is %s\n", (flags & O_APPEND) ? "on" : "off");

	switch(cpid = fork()){
		case -1:
			fprintf(stderr,"fork error");
			exit(EXIT_SUCCESS);
			break;
		case 0:
			offset = lseek(fd,1000,SEEK_CUR);
			flags |= O_APPEND;
			fcntl(fd,F_SETFL,flags);
			break;
		default:
			wait(NULL);
			printf("Child exited\n");
			offset = lseek(fd,0,SEEK_CUR);
			printf("FILE offset before fork(): %lld\n",(long long)offset);
			flags = fcntl(fd,F_GETFL);
			printf("O_APPEND flag before fork() is %s\n", (flags & O_APPEND) ? "on" : "off");
			break;
	}

	return 0;
}
