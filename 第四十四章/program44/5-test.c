#include <stdio.h>
#include <unistd.h>

int main(){

	execlp("ls","ls","-l"/*,"test*"*/,NULL);
	//execlp("ls","ls","-d",NULL);
	//execlp("pwd","pwd",NULL);

	return 0;
}
