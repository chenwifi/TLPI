#include "tlpi_hdr.h"
#include <fcntl.h>

int main(int argc,char *argv[]){
	int i,fd,j;
	long res;
	char *mptr;
	off_t ret;
	ssize_t numRead;

	if(argc < 3 || strcmp(argv[1],"--help") == 0){
		usageErr("%s file {r<length>|R<length>|s<offset>|w<str>}");
	}

	fd = open(argv[1],O_RDWR | O_CREAT,0666);
	if(fd == -1){
		errExit("opening file %s",argv[1]);
	}

	for(i = 2;i<argc;i++){
		switch(argv[i][0]){
			case 'r':
			case 'R':
				res = getLong(&argv[i][1],GN_NONNEG | GN_ANY_BASE,argv[i]);

				mptr = (char *)malloc(res);
				if(mptr == NULL){
					errExit("malloc");
				}

				if((numRead = read(fd,mptr,res)) == -1){
					errExit("read file %s",argv[1]);
				}

				if(numRead == 0){
					printf("%s: end of file",argv[i]);
				}else{
					/*
					if(argv[i][0] == 'r'){
						printf("%s",mptr);
					}else{
						printf("%x",(int *)mptr);
					}
					*/
					for(j = 0;j<numRead;j++){
						if(argv[i][0] == 'r'){
							printf("%c",isprint(mptr[j]) ? mptr[j] : '?');
						}else{
							printf("%02x",(unsigned int)mptr[j]);
						}
					}
					printf("\n");
				}
				free(mptr);
				break;

			case 's':
				res = getLong(&argv[i][1],GN_NONNEG | GN_ANY_BASE,argv[i]);
				ret = lseek(fd,res,SEEK_SET);
				if(ret == -1){
					errExit("lseek file %s",argv[1]);
				}

				break;
			case 'w':
				if(write(fd,&argv[i][1],strlen(&argv[i][1])) == -1){
					errExit("write file %s",argv[1]);
				}
				break;
			default:
				cmdLineErr("Argument must start with [rRsw]:%s\n",argv[i]);
		}
	}
}
