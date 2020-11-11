#include <stdio.h>

__asm__(".symver xyz_1,xyz@VER_1");
__asm__(".symver xyz_2,xyz@@VER_2");

void xyz_1(){
	printf("v1\n");
}

void xyz_2(){
	printf("v2\n");
}

void pqr(){
	printf("pqr\n");
}
