#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>
#include <time.h>

int main(int argc,char *argv[]){
	struct stat sb;

	if(argc != 2){
		fprintf(stderr,"Usage: %s <pathname>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	if(stat(argv[1],&sb) == -1){
		exit(EXIT_FAILURE);
	}

	printf("last access time: %s\n",ctime(&sb.st_atime));
	printf("last modify time: %s\n",ctime(&sb.st_mtime));

	if(utime(argv[1],NULL) == -1){
		exit(EXIT_FAILURE);
	}

	if(stat(argv[1],&sb) == -1){
		exit(EXIT_FAILURE);
	}

	printf("last access time: %s\n",ctime(&sb.st_atime));
	printf("last modify time: %s\n",ctime(&sb.st_mtime));

	return 0;
}
