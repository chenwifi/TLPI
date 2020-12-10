#include <stdio.h>

int main(int argc,char *argv[]){
	int *a;
	int b[10];

	b[0] = 10;
	a = b;
	printf("%d\n",a[0]);

	return 0;
}
