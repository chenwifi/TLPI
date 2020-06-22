#define _BSD_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	char buf[250];
	int fd;

	mkdir("bigfat",0777);
	chroot("bigfat");
	chdir("/");
	mkdir("sub",0777);

	fd = open("../test1",O_RDONLY | O_CREAT,0777);
	printf("fd: %d\n",fd);
	getcwd(buf,250);

	printf("current dir:%s\n",buf);

	return 0;
}
