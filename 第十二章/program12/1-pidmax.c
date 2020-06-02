#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_MAX 100

int main(int argc,char *argv[]){
	int fd;
	ssize_t t;
	char buf[BUF_MAX];

	fd = open("/proc/sys/kernel/pid_max",O_RDONLY);
	if(fd == -1){
		exit(EXIT_FAILURE);
	}

	t = read(fd,buf,BUF_MAX);
	if(t == -1){
		exit(EXIT_FAILURE);
	}
	printf("%.*s",(int)t,buf);

	return 0;
}
