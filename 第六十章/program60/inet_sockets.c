#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "inet_sockets.h"

int inetConnect(const char *host,const char *service,int type){
	struct addrinfo hints;
	struct addrinfo *res,*tmp;
	int cfd;

	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = type;
	hints.ai_addr = NULL;
	hints.ai_canonname = NULL;
	hints.ai_next = NULL;

	if(getaddrinfo(host,service,&hints,&res) != 0){
		fprintf(stderr,"getaddrinfo\n");
		exit(EXIT_FAILURE);
	}

	for(tmp = res; tmp != NULL; tmp = tmp->ai_next){
		cfd = socket(tmp->ai_family,tmp->ai_socktype,tmp->ai_protocol);
		if(cfd == -1){
			continue;
		}

		if(connect(cfd,tmp->ai_addr,tmp->ai_addrlen) != -1){
			break;
		}

		close(cfd);
	}

	freeaddrinfo(res);

	return tmp == NULL ? -1 : cfd;
}

static int inetPassiveSocket(const char *service,int type,socklen_t *addrlen,int doListen,int backlog){
	struct addrinfo hints;
	struct addrinfo *res,*tmp;
	int sfd,optval;

	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = type;
	hints.ai_addr = NULL;
	hints.ai_canonname = NULL;
	hints.ai_next = NULL;

	if(getaddrinfo(NULL,service,&hints,&res) != 0){
		fprintf(stderr,"getaddrinfo\n");
		exit(EXIT_FAILURE);
	}

	optval = 1;
	for(tmp = res; tmp != NULL; tmp = tmp->ai_next){
		sfd = socket(tmp->ai_family,tmp->ai_socktype,tmp->ai_protocol);
		if(sfd == -1){
			continue;
		}

		if(doListen){
			if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval)) == -1){
				close(sfd);
				freeaddrinfo(res);
				fprintf(stderr,"setsockopt\n");
				exit(EXIT_FAILURE);

			}
		}

		if(bind(sfd,tmp->ai_addr,tmp->ai_addrlen) == 0){
			break;
		}

		close(sfd);
	}

	if(tmp == NULL){
		freeaddrinfo(res);
		fprintf(stderr,"can not get address\n");
		exit(EXIT_FAILURE);
	}


	if(addrlen != NULL){
		*addrlen = tmp->ai_addrlen;
	}

	if(doListen){
		if(listen(sfd,backlog) == -1){
			freeaddrinfo(res);
			fprintf(stderr,"listen\n");
			exit(EXIT_FAILURE);
		}
	}

	freeaddrinfo(res);

	return sfd;
}

int inetListen(const char *service,int backlog,socklen_t *addrlen){
	return inetPassiveSocket(service,SOCK_STREAM,addrlen,1,backlog);
}

int inetBind(const char *service,int type,socklen_t *addrlen){
	return inetPassiveSocket(service,type,addrlen,0,0);
}

char *inetAddressStr(const struct sockaddr *addr,socklen_t addrlen,char *addrStr,int addrStrLen){
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	if(getnameinfo(addr,addrlen,host,NI_MAXHOST,service,NI_MAXSERV,0) != 0){
		snprintf(addrStr,addrStrLen,"%s","(UNKNOW)");
	}else{
		snprintf(addrStr,addrStrLen,"(%s,%s)",host,service);
	}

	addrStr[addrStrLen - 1] = '\0';

	return addrStr;
}
