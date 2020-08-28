#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
	int fd;

	fd = open("test1",O_RDWR);
	write(fd,"h",1);

	sleep(10);
}
