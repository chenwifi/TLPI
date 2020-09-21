#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>

int main(int argc,char *argv[]){

	printf("tid=%ld\n",(long)syscall(SYS_gettid));

	return 0;
}
