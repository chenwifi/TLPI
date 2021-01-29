#include "2-us_xfr.h"
#include <errno.h>

int main(int argc,char *argv[]){
	int sfd,cfd;
	ssize_t numRead;
	struct sockaddr_un uaddr;
	char buf[BUF_SIZE];

	sfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(sfd == -1){
		fprintf(stderr,"socket\n");
		exit(EXIT_FAILURE);
	}

	if(remove(SV_SOCK_PATH) == -1 && errno != ENOENT){
		fprintf(stderr,"remove\n");
		exit(EXIT_FAILURE);
	}

	memset(&uaddr,0,sizeof(struct sockaddr_un));
	uaddr.sun_family = AF_UNIX;
	snprintf(uaddr.sun_path,sizeof(uaddr.sun_path) - 1,"%s",SV_SOCK_PATH);

	if(bind(sfd,(struct sockaddr *)&uaddr,sizeof(struct sockaddr_un)) == -1){
		fprintf(stderr,"bind\n");
		exit(EXIT_FAILURE);
	}

	if(listen(sfd,5) == -1){
		fprintf(stderr,"listen\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		cfd = accept(sfd,NULL,0);
		if(cfd == -1){
			fprintf(stderr,"accept\n");
			exit(EXIT_FAILURE);
		}

		while((numRead = read(cfd,buf,BUF_SIZE)) > 0){
			if(write(STDOUT_FILENO,buf,numRead) != numRead){
				fprintf(stderr,"write\n");
				exit(EXIT_FAILURE);
			}
		}

		if(numRead == -1){
			fprintf(stderr,"read\n");
			exit(EXIT_FAILURE);
		}

		close(cfd);
	}

	return 0;
}
