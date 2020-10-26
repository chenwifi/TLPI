#include <stdio.h>
#include <unistd.h>

int main(){
	close(0);
	close(1);
	//close(2);

	printf("hello world\n");

	return 0;
}
