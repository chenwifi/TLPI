#include "tlpi_hdr.h"
#include <sys/uio.h>
#include <string.h>

int main(){
	/*
	char *str0 = "hello ";
	char *str1 = "world!\n";

	struct iovec myiov[2];
	myiov[0].iov_base = str0;
	myiov[0].iov_len = strlen(str0);
	myiov[1].iov_base = str1;
	myiov[1].iov_len = strlen(str1);

	writev(1,myiov,2);
	*/

	int readNum;

	struct iovec myiov[2];
	char a[10];
	char b[10];

	myiov[0].iov_base = a;
	myiov[0].iov_len = 10;
	myiov[1].iov_base = b;
	myiov[1].iov_len = 10;

	readNum = readv(0,myiov,2);
	a[9] = '\0';
	b[9] = '\0';

	printf("a is %s and b is %s\n",a,b);
}
