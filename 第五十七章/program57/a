#include "2-us_xfr.h"

int main(int argc,char *argv[]){
	int cfd;
	struct sockaddr_un uaddr;
	ssize_t numRead;
	char buf[BUF_SIZE];

	cfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(cfd == -1){
		fprintf(stderr,"socket\n");
		exit(EXIT_FAILURE);
	}

	memset(&uaddr,0,sizeof(struct sockaddr_un));
	uaddr.sun_family = AF_UNIX;
	snprintf(uaddr.sun_path,sizeof(uaddr.sun_path) - 1,"%s",SV_SOCK_PATH);

	if(connect(cfd,(struct sockaddr *)&uaddr,sizeof(struct sockaddr_un)) == -1){
		fprintf(stderr,"connect\n");
		exit(EXIT_FAILURE);
	}

	while((numRead = read(STDIN_FILENO,buf,BUF_SIZE)) > 0){
		if(write(cfd,buf,numRead) != numRead){
			fprintf(stderr,"write\n");
			exit(EXIT_FAILURE);
		}
	}

	if(numRead == -1){
		fprintf(stderr,"read\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
