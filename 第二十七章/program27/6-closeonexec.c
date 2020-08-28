#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc,char *argv[]){
	int flags;

	if(argc > 1){
		flags = fcntl(STDOUT_FILENO,F_GETFD);
		flags |= FD_CLOEXEC;
		fcntl(STDOUT_FILENO,F_SETFD,flags);
	}

	execlp("ls","ls","-l",argv[0],(char *)NULL);

	return 0;
}
