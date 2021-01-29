#define _BSD_SOURCE
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc,char *argv[]){
	struct hostent *h;
	char **pp;
	char str[INET6_ADDRSTRLEN];

	for(argv++; *argv != NULL; argv++){
		h = gethostbyname(*argv);
		if(h == NULL){
			fprintf(stderr,"gethostbyname fail for %s : %s\n",*argv,hstrerror(h_errno));
			continue;
		}

		printf("Canonical name: %s\n",h->h_name);
		printf("			alias:			");
		for(pp = h->h_aliases; *pp != NULL; pp++){
			printf(" %s",*pp);
		}
		printf("\n");
		printf("		address type:		%s\n",(h->h_addrtype == AF_INET) ? "AF_INET" : (h->h_addrtype == AF_INET6) ? "AF_INET6" : "???");

		if(h->h_addrtype == AF_INET || h->h_addrtype == AF_INET6){
			printf("		adress		");
			for(pp = h->h_addr_list; *pp != NULL; pp++){
				if(inet_ntop(h->h_addrtype,*pp,str,INET6_ADDRSTRLEN) == NULL){
					fprintf(stderr,"inet_ntop\n");
					exit(EXIT_FAILURE);
				}
				printf(" %s",inet_ntop(h->h_addrtype,*pp,str,INET6_ADDRSTRLEN));
			}
		}
		printf("\n");
	}

	return 0;
}
