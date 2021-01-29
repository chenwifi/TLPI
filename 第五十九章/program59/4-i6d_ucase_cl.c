#include "2-i6d_ucase.h"

int main(int argc,char *argv[]){
	int cfd,i;
	size_t msgLen;
	ssize_t numBytes;
	struct sockaddr_in6 svaddr;
	char buf[BUF_SIZE];

	if(argc < 3 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s host-address msg...\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	cfd = socket(AF_INET6,SOCK_DGRAM,0);
	if(cfd == -1){
		fprintf(stderr,"socket\n");
		exit(EXIT_FAILURE);
	}

	memset(&svaddr,0,sizeof(struct sockaddr_in6));
	svaddr.sin6_port = htons(PORT_NUM);
	svaddr.sin6_family = AF_INET6;
	if(inet_pton(AF_INET6,argv[1],&svaddr.sin6_addr) != 1){
		fprintf(stderr,"inet_pton\n");
		exit(EXIT_FAILURE);
	}

	for(i = 2; i < argc; i++){
		msgLen = strlen(argv[i]);
		if(sendto(cfd,argv[i],msgLen,0,(struct sockaddr *)&svaddr,sizeof(struct sockaddr_in6)) != msgLen){
			fprintf(stderr,"sendto\n");
			exit(EXIT_FAILURE);
		}

		if((numBytes = recvfrom(cfd,buf,BUF_SIZE,0,NULL,NULL)) == -1){
			fprintf(stderr,"recvfrom\n");
			exit(EXIT_FAILURE);
		}

		printf("Response %d: %.*s\n",i - 1,numBytes,buf);
	}

	return 0;
}
