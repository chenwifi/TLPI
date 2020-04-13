#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
	int fd,fd2,fd3,fd4,fd5;
	FILE *fp,*fp1;

	//close(1);
	fclose(stdout);
	fd = open("test",O_WRONLY);
	fd2 = fileno(stdout);
	//fp = fopen("test1","w");
	printf("fd num is %d---%d\n",fd,fd2);
	fd5 = fileno(stdout);

	//dup2(fd,1);
	//fp1 = freopen("test1","w",stdout);
	//close(fd);
	fd3 = fileno(stdout);
	//fd4 = fileno(fp1);

	printf("num is %d----%d-----%d----%d----\n",fd,fd2,fd3,fd5);
	//write(1,"iambigfat",20);

	return 0;
}
