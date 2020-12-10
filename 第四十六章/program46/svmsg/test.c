#include <stdio.h>
#include <string.h>

int main(){
	char a[100];
	snprintf(a,100,"hello world");

	int len = sizeof(a);

	printf("%d\n",len);

	return 0;
}
