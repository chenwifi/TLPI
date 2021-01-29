#define _GNU_SOURCE
#include <signal.h>
#include "5-is_seqnum.h"

#define BACKLOG 50

int main(int argc,char *argv[]){
	uint32_t seqNum;
	int sfd,cfd,reqLen,optval;
	struct addrinfo hints;
	struct addrinfo *res,*tmp;
	struct sockaddr_storage claddr;
	char clhost[NI_MAXHOST];
	char clserv[NI_MAXSERV];
#define HSLEN NI_MAXHOST + NI_MAXSERV + 10
	char clhs[HSLEN];
	char clNum[INT_LEN];
	char seqNumStr[INT_LEN];
	socklen_t addrLen;

	if(argc > 1 && strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s [init-num]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	seqNum = argc > 1 ? atoi(argv[1]) : 0;

	if(signal(SIGPIPE,SIG_IGN) == SIG_ERR){
		fprintf(stderr,"signal\n");
		exit(EXIT_FAILURE);
	}

	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_addr = NULL;
	hints.ai_canonname = NULL;
	hints.ai_next = NULL;

	if(getaddrinfo(NULL,PORT_NUM,&hints,&res) != 0){
		fprintf(stderr,"getaddrinfo\n");
		exit(EXIT_FAILURE);
	}

	optval = 1;
	for(tmp = res; tmp != NULL; tmp = tmp->ai_next){
		if((sfd = socket(tmp->ai_family,tmp->ai_socktype,tmp->ai_protocol)) == -1){
			fprintf(stderr,"socket\n");
			exit(EXIT_FAILURE);
		}

		//always set: and why??
		if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval)) == -1){
		}

		if(bind(sfd,tmp->ai_addr,tmp->ai_addrlen) == 0){
			break;
		}

		close(sfd);
	}

	if(tmp == NULL){
		fprintf(stderr,"can not get address\n");
		exit(EXIT_FAILURE);
	}

	if(listen(sfd,BACKLOG) == -1){
		fprintf(stderr,"listen:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(res);

	addrLen = sizeof(struct sockaddr_storage);
	while(1){
		if((cfd = accept(sfd,(struct sockaddr *)&claddr,&addrLen)) == -1){
			fprintf(stderr,"accept\n");
			continue;
		}

		if(getnameinfo((struct sockaddr *)&claddr,sizeof(struct sockaddr_storage),clhost,NI_MAXHOST,clserv,NI_MAXSERV,0) != 0){
			snprintf(clhs,HSLEN,"%s","???UNKNOW???");
		}else{
			snprintf(clhs,HSLEN,"(%s,%s)",clhost,clserv);
		}

		printf("connected from %s\n",clhs);

		if(readLine(cfd,clNum,INT_LEN) <= 0){
			close(cfd);
			continue;
		}

		reqLen = atoi(clNum);
		if(reqLen <= 0){
			close(cfd);
			continue;
		}

		snprintf(seqNumStr,INT_LEN,"%d\n",seqNum);
		if(write(cfd,seqNumStr,strlen(seqNumStr)) != strlen(seqNumStr)){
			fprintf(stderr,"accept\n");
			exit(EXIT_FAILURE);
		}

		seqNum += reqLen;

		if(close(cfd) == -1){
			fprintf(stderr,"close\n");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
