#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "inet_sockets.h"

int main(int argc,char *argv[]){
	int listenFd,acceptFd,connFd;
	socklen_t len;
	void *addr;
	char addrStr[IS_ADDR_STR_LEN];

	if(argc != 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s service\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	listenFd = inetListen(argv[1],5,&len);
	if(listenFd == -1){
		fprintf(stderr,"inetListen\n");
		exit(EXIT_FAILURE);
	}

	connFd = inetConnect(NULL,argv[1],SOCK_STREAM);
	if(connFd == -1){
		fprintf(stderr,"inetConnect\n");
		exit(EXIT_FAILURE);
	}

	acceptFd = accept(listenFd,NULL,NULL);
	if(acceptFd == -1){
		fprintf(stderr,"accept\n");
		exit(EXIT_FAILURE);
	}

	addr = malloc(len);
	if(addr == NULL){
		fprintf(stderr,"malloc\n");
		exit(EXIT_FAILURE);
	}

	if(getsockname(connFd,(struct sockaddr *)addr,&len) != 0){
		fprintf(stderr,"getsockname\n");
		exit(EXIT_FAILURE);
	}

	printf("getsockname(connfd): %s\n",inetAddressStr(addr,len,addrStr,IS_ADDR_STR_LEN));

	if(getsockname(acceptFd,(struct sockaddr *)addr,&len) != 0){
		fprintf(stderr,"getsockname\n");
		exit(EXIT_FAILURE);
	}
	printf("getsockname(acceptFd): %s\n",inetAddressStr(addr,len,addrStr,IS_ADDR_STR_LEN));

	if(getpeername(connFd,(struct sockaddr *)addr,&len) != 0){
		fprintf(stderr,"getpeername\n");
		exit(EXIT_FAILURE);
	}
	printf("getpeername(connFd): %s\n",inetAddressStr(addr,len,addrStr,IS_ADDR_STR_LEN));

	if(getpeername(acceptFd,(struct sockaddr *)addr,&len) != 0){
		fprintf(stderr,"getpeername\n");
		exit(EXIT_FAILURE);
	}
	printf("getpeername(acceptFd): %s\n",inetAddressStr(addr,len,addrStr,IS_ADDR_STR_LEN));

	return 0;
}
