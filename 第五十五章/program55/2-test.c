#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(){
	int fd;
	char buf[100];

	memset(buf,0,100);

	fd = open("tfile",O_RDWR);
	write(fd,buf,100);

	return 0;
}
