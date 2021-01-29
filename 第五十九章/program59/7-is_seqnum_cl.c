#include "5-is_seqnum.h"

int main(int argc,char *argv[]){
	struct addrinfo hints;
	struct addrinfo *res,*tmp;
	int sfd;
	int seqLen;
	char seqLenStr[INT_LEN];
	char retNum[INT_LEN];

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s server-host [seq-len]\n");
		exit(EXIT_FAILURE);
	}

	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_flags = AI_NUMERICSERV;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_addr = NULL;
	hints.ai_canonname = NULL;
	hints.ai_next = NULL;

	if(getaddrinfo(argv[1],PORT_NUM,&hints,&res) != 0){
		fprintf(stderr,"getaddrinfo\n");
		exit(EXIT_FAILURE);
	}

	for(tmp = res; tmp != NULL; tmp = tmp->ai_next){
		sfd = socket(tmp->ai_family,tmp->ai_socktype,tmp->ai_protocol);
		if(sfd == -1){
			fprintf(stderr,"socket\n");
			continue;
		}

		if(connect(sfd,tmp->ai_addr,tmp->ai_addrlen) == 0){
			break;
		}

		close(sfd);
	}

	if(tmp == NULL){
		fprintf(stderr,"could not connect address\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(res);

	seqLen = (argc > 2) ? atoi(argv[2]) : 1;

	snprintf(seqLenStr,INT_LEN,"%d\n",seqLen);
	if(write(sfd,seqLenStr,strlen(seqLenStr)) != strlen(seqLenStr)){
		fprintf(stderr,"write:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	if(readLine(sfd,retNum,INT_LEN) <= 0){
		fprintf(stderr,"readLine\n");
		exit(EXIT_FAILURE);
	}

	printf("Response: %s",retNum);

	return 0;
}
