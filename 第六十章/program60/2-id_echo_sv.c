#include <stdio.h>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "become_daemon.h"
#include "inet_sockets.h"
#include "1-id_echo.h"

int main(int argc,char *argv[]){
	int sfd;
	struct sockaddr_storage claddr;
	socklen_t len;
	ssize_t numReads;
	char buf[BUF_SIZE];
	char addrStr[IS_ADDR_STR_LEN];

	if(becomeDaemon(0) == -1){
		fprintf(stderr,"becomeDaemon\n");
		exit(EXIT_FAILURE);
	}

	if((sfd = inetBind(SERVICE,SOCK_DGRAM,NULL)) == -1){
		fprintf(stderr,"inetBind\n");
		exit(EXIT_FAILURE);
	}

	len = sizeof(struct sockaddr_storage);
	while(1){
		numReads = recvfrom(sfd,buf,BUF_SIZE,0,(struct sockaddr *)&claddr,&len);
		if(numReads < 0){
			fprintf(stderr,"recvfrom\n");
			exit(EXIT_FAILURE);
		}

		//printf("address: %s\n",inetAddressStr((struct sockaddr *)&claddr,len,addrStr,IS_ADDR_STR_LEN));
		//printf("get str: %s\n",buf);

		if(sendto(sfd,buf,numReads,0,(struct sockaddr *)&claddr,len) != numReads){
			syslog(LOG_WARNING,"Error echo response to %s (%s)",inetAddressStr((struct sockaddr *)&claddr,len,addrStr,IS_ADDR_STR_LEN),strerror(errno));
		}
	}

	return 0;
}
