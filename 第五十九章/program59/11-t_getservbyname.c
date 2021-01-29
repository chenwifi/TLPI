#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
	struct servent *s;
	char **pp;

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s service [protoclo]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	if((s = getservbyname(argv[1],argv[2])) == NULL){
		fprintf(stderr,"getservbyname\n");
		exit(EXIT_FAILURE);
	}

	printf("Official name: %s\n",s->s_name);
	printf("aliases: ");
	for(pp = s->s_aliases; *pp != NULL; pp++){
		printf(" %s",*pp);
	}
	printf("\n");
	printf("PORT: %u\n",ntohs(s->s_port));
	printf("protoclo: %s\n",s->s_proto);

	return 0;
}
