#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>

static void listFiles(const char *dirpath);


int main(int argc,char *argv[]){
	int i;

	if(argc > 1 && strcmp(argv[1],"--help") == 0)
		fprintf(stderr,"Usage: %s [dirname]\n",argv[0]);

	if(argc == 1){
		listFiles(".");
	}else{
		for(i = 1; i < argc; i++){
			listFiles(argv[i]);
		}
	}

	return 0;
}

static void listFiles(const char *dirpath){
	DIR *dirp;
	struct dirent *direntp;
	int isCurrent;

	if((dirp = opendir(dirpath)) == NULL){
		fprintf(stderr,"opendir\n");
		exit(EXIT_FAILURE);
	}

	isCurrent = (strcmp(dirpath,".") == 0) ? 1 : 0;

	errno = 0;
	while(1){
		direntp = readdir(dirp);

		if(direntp != NULL){
			if(strcmp(direntp->d_name,".") == 0 || strcmp(direntp->d_name,"..") == 0){
				continue;
			}

			if(isCurrent){
				printf("./");
			}

			printf("%s\n",direntp->d_name);
		}else{
			if(errno != 0){
				fprintf(stderr,"readdir\n");
				exit(EXIT_FAILURE);
			}else{
				break;
			}
		}
	}
}
