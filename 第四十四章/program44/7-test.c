#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
	int fd,fd2;
	char a[10];
	ssize_t len;

	mkfifo("myfifo",S_IRUSR | S_IWUSR | S_IWGRP);
	fd = open("myfifo",O_RDONLY | O_NONBLOCK);
	if(fd == -1){
		fprintf(stderr,"%s\n",strerror(errno));
	}
	fd2 = open("myfifo",O_WRONLY | O_NONBLOCK);

	len = read(fd,&a,1);
	if(len == 0){
		printf("len is zero\n");
	}

	if(len == -1){
		printf("%s",strerror(errno));
	}

	return 0;
}
