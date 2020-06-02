#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 100

int main(){
	static char buf[BUF_SIZE];
	int a;

	setvbuf(stdout,buf,_IOFBF,BUF_SIZE);
	printf("abcdefg");

	//scanf("%d",&a);
	//sleep(3);
	printf("bigfat");
	write(1,"wifi",4);

	return 0;
}
