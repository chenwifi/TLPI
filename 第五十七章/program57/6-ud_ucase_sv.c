#include "5-ud_ucase.h"
#include <errno.h>
#include <ctype.h>

int main(int argc,char *argv[]){
	int sfd;
	struct sockaddr_un uaddr,ucladdr;
	ssize_t numRecv,numSend;
	char buf[BUF_SIZE];
	socklen_t len;
	int i;

	sfd = socket(AF_UNIX,SOCK_DGRAM,0);
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

	while(1){
		len = sizeof(struct sockaddr_un);
		if((numRecv = recvfrom(sfd,buf,BUF_SIZE,0,(struct sockaddr *)&ucladdr,&len)) == -1){
			fprintf(stderr,"recvfrom\n");
			exit(EXIT_FAILURE);
		}

		printf("receive %ld bytes form %s\n",numRecv,ucladdr.sun_path);

		for(i = 0; i < numRecv; i++){
			buf[i] = toupper(buf[i]);
		}

		if((numSend = sendto(sfd,buf,numRecv,0,(struct sockaddr *)&ucladdr,len)) != numRecv){
			fprintf(stderr,"sendto\n");
			exit(EXIT_FAILURE);
		}
	}
}
