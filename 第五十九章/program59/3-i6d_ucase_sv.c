#include "2-i6d_ucase.h"

int main(int argc,char *argv[]){
	struct sockaddr_in6 svaddr,claddr;
	socklen_t len;
	ssize_t numBytes;
	char buf[BUF_SIZE];
	int sfd,i;
	char claddrStr[INET6_ADDRSTRLEN];

	sfd = socket(AF_INET6,SOCK_DGRAM,0);
	if(sfd == -1){
		fprintf(stderr,"socket\n");
		exit(EXIT_FAILURE);
	}

	memset(&svaddr,0,sizeof(struct sockaddr_in6));
	svaddr.sin6_family = AF_INET6;
	svaddr.sin6_port = htons(PORT_NUM);
	svaddr.sin6_addr = in6addr_any;

	if(bind(sfd,(struct sockaddr *)&svaddr,sizeof(struct sockaddr_in6)) == -1){
		fprintf(stderr,"bind\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		len = sizeof(struct sockaddr_in6);
		numBytes = recvfrom(sfd,buf,BUF_SIZE,0,(struct sockaddr *)&claddr,&len);
		if(numBytes == -1){
			fprintf(stderr,"recvfrom\n");
			exit(EXIT_FAILURE);
		}

		if(inet_ntop(AF_INET6,&claddr.sin6_addr,claddrStr,INET_ADDRSTRLEN) == NULL){
			printf("Could not convert client address to string\n");
		}else{
			printf("Server received %ld bytes from (%s,%u)\n",(long)numBytes,claddrStr,ntohs(claddr.sin6_port));
		}

		for(i = 0; i < numBytes; i++){
			buf[i] = toupper(buf[i]);
		}

		if(sendto(sfd,buf,numBytes,0,(struct sockaddr *)&claddr,len) != numBytes){
			fprintf(stderr,"sendto\n");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
