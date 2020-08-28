#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
int main(){

	sleep(5);
	_exit(4);
}
*/

static void func1(){
	printf("atexit func 1 called\n");
}

static void func2(){
	printf("atexit func 2 called\n");
}

static void func3(int status,void *arg){
	printf("status=%d arg=%s\n",status,arg);
}

int main(){
	//int *a = (int *)10;
	char a[] = "chenweihui";
	on_exit(func3,(void *)a);
	atexit(func1);
	atexit(func2);
	on_exit(func3,(void *)30);

	exit(5);
	//return 3;
}
