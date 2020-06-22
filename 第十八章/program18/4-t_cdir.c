#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DIR_MAX 250

int main(){

	int fd;
	fd = open("/root/TLPI/program18",O_RDONLY);
	printf("%d\n",fd);

	/*
	char buf[DIR_MAX];

	getcwd(buf,DIR_MAX);
	printf("current pwd %s\n",buf);

	chdir("..");
	getcwd(buf,DIR_MAX);
	printf("current pwd %s\n",buf);
	*/

	return 0;
}
