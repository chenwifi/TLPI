#define _BSD_SOURCE
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUF_SIZE PATH_MAX

int main(int argc,char *argv[]){
	char buf[BUF_SIZE];
	struct stat st;
	ssize_t len;

	if(lstat(argv[1],&st) == -1){
		fprintf(stderr,"lstat\n");
		exit(EXIT_FAILURE);
	}

	if(!S_ISLNK(st.st_mode)){
		fprintf(stderr,"not a symbolic link\n");
		exit(EXIT_FAILURE);
	}

	len = readlink(argv[1],buf,BUF_SIZE - 1);
	if(len == -1){
		exit(EXIT_FAILURE);
	}

	buf[len] = '\0';

	printf("%s -> %s\n",argv[1],buf);

	realpath(argv[1],buf);
	printf("%s -> %s\n",argv[1],buf);

	return 0;
}
