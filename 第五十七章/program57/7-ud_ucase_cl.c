#include "5-ud_ucase.h"
#include <errno.h>

int main(int argc,char *argv[]){
	int sfd,i;
	struct sockaddr_un uaddr,usvaddr;
	size_t msgLen;
	ssize_t recvLen;
	char buf[BUF_SIZE];

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s msg...\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	sfd = socket(AF_UNIX,SOCK_DGRAM,0);
	if(sfd == -1){
		fprintf(stderr,"socket\n");
		exit(EXIT_FAILURE);
	}

	memset(&uaddr,0,sizeof(struct sockaddr_un));
	uaddr.sun_family = AF_UNIX;
	snprintf(uaddr.sun_path,sizeof(uaddr.sun_path) - 1,"/tmp/ud_ucase_cl.%ld",(long)getpid());

	if(bind(sfd,(struct sockaddr *)&uaddr,sizeof(struct sockaddr_un)) == -1){
		fprintf(stderr,"bind\n");
		exit(EXIT_FAILURE);
	}

	memset(&usvaddr,0,sizeof(struct sockaddr_un));
	usvaddr.sun_family = AF_UNIX;
	snprintf(usvaddr.sun_path,sizeof(usvaddr.sun_path) - 1,"%s",SV_SOCK_PATH);

	for(i = 1; i < argc; i++){
		msgLen = strlen(argv[i]);
		if(sendto(sfd,argv[i],msgLen,0,(struct sockaddr *)&usvaddr,sizeof(struct sockaddr_un)) != msgLen){
			fprintf(stderr,"sendto:%s\n",strerror(errno));
			exit(EXIT_FAILURE);
		}

		if((recvLen = recvfrom(sfd,buf,BUF_SIZE,0,NULL,NULL)) == -1){
			fprintf(stderr,"recvfrom\n");
			exit(EXIT_FAILURE);
		}

		printf("Response %d: %.*s\n",i,recvLen,buf);
	}

	remove(uaddr.sun_path);

	return 0;
}
