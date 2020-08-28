#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
	printf("initial value of user : %s\n",getenv("USER"));

	putenv("USER=BBIIGG");

	execlp("printenv","printenv","USER",(char *)NULL);

	return 0;
}
