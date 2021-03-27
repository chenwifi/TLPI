#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "inet_sockets.h"

#define BUF_SIZE 100

int main(int argc,char *argv[]){
	int sfd;
	ssize_t numRead;
	char buf[BUF_SIZE];

	if(argc != 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s host\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	sfd = inetConnect(argv[1],"echo",SOCK_STREAM);
	if(sfd == -1){
		fprintf(stderr,"inetConnect\n");
		exit(EXIT_FAILURE);
	}

	switch(fork()){
		case -1:
			fprintf(stderr,"fork\n");
			exit(EXIT_FAILURE);
			break;
		case 0:
			while(1){
				numRead = read(sfd,buf,BUF_SIZE);
				if(numRead <= 0){
					break;
				}
				printf("%.*s\n",numRead,buf);
			}
			break;
		default:
			while(1){
				numRead = read(STDIN_FILENO,buf,BUF_SIZE);
				if(numRead <= 0){
					break;
				}

				if(write(sfd,buf,numRead) != numRead){
					fprintf(stderr,"write\n");
					exit(EXIT_FAILURE);
				}
			}

			if(shutdown(sfd,SHUT_WR) == -1){
				fprintf(stderr,"shutdown\n");
				exit(EXIT_FAILURE);
			}

			break;
	}

	return 0;
}
