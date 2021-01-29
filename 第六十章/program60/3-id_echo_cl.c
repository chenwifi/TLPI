#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "1-id_echo.h"
#include "inet_sockets.h"

int main(int argc,char *argv[]){
	int cfd,i;
	size_t len;
	char buf[BUF_SIZE];
	ssize_t numRead;

	if(argc < 3 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s host msg...\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	if((cfd = inetConnect(argv[1],SERVICE,SOCK_DGRAM)) == -1){
		fprintf(stderr,"inetConnect\n");
		exit(EXIT_FAILURE);
	}

	for(i = 2; i < argc; i++){
		len = strlen(argv[i]);
		if(write(cfd,argv[i],len) != len){
			fprintf(stderr,"write\n");
			exit(EXIT_FAILURE);
		}

		if((numRead = read(cfd,buf,BUF_SIZE)) < 0){
			fprintf(stderr,"read\n");
			exit(EXIT_FAILURE);
		}

		printf("getRes : %.*s\n",numRead,buf);
	}

	return 0;
}
