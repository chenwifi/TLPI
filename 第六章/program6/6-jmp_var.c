#include <stdio.h>
#include <setjmp.h>

static jmp_buf env;

static test(int a,int b,int c){
	printf("a is %d,b is %d,c is %d\n",a,b,c);
	longjmp(env,1);
}

int main(){
	int a;
	register int b;
	volatile int c;

	a = 111;
	b = 222;
	c = 333;

	if(!setjmp(env)){
		a = 444;
		b = 555;
		c = 666;
		test(a,b,c);
	}else{
		printf("from longjmp ");
		printf("a is %d,b is %d,c is %d\n",a,b,c);
	}
}
