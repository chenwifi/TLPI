#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char *argv[]){
	int fd;

	fd = open("6-test",O_RDONLY);
	printf("%d\n",fd);
	unlink("6-test");
	fd = open("6-test",O_RDONLY);
	printf("%d\n",fd);

	return 0;
}
