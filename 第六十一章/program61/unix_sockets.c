#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "unix_sockets.h"

int unixBuildAddress(const char *path,struct sockaddr_un *addr){
	if(path == NULL || addr == NULL){
		errno = EINVAL;
		return -1;
	}
	/*
	if(remove(path) != 0 && errno != ENOENT){
		fprintf(stderr,"remove\n");
		exit(EXIT_FAILURE);
	}
	*/

	memset(addr,0,sizeof(struct sockaddr_un)); 
	addr->sun_family = AF_UNIX;
	if(strlen(path) < sizeof(addr->sun_path)){
		strncpy(addr->sun_path,path,sizeof(addr->sun_path) - 1);
		return 0;
	}else{
		errno = ENAMETOOLONG;
		return -1;
	}
}

int unixConnect(const char *path,int type){
	int sfd;
	struct sockaddr_un addr;

	if(unixBuildAddress(path,&addr) != 0){
		fprintf(stderr,"unixBuildAddress\n");
		exit(EXIT_FAILURE);
	}

	if((sfd = socket(AF_UNIX,type,0)) == -1){
		fprintf(stderr,"socket\n");
		exit(EXIT_FAILURE);
	}

	if(connect(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr)) != -1){
		fprintf(stderr,"connect\n");
		exit(EXIT_FAILURE);
	}

	return sfd;
}

int unixBind(const char *path,int type){
	int sfd;
	struct sockaddr_un addr;
	socklen_t len;

	if((sfd = socket(AF_UNIX,type,0)) == -1){
		fprintf(stderr,"socket\n");
		exit(EXIT_FAILURE);
	}

	if(unixBuildAddress(path,&addr) != 0){
		fprintf(stderr,"unixBuildAddress\n");
		exit(EXIT_FAILURE);
	}

	len = sizeof(struct sockaddr_un);
	if(bind(sfd,(struct sockaddr *)&addr,len) == -1){
		fprintf(stderr,"bind\n");
		exit(EXIT_FAILURE);
	}

	return fd;
}
