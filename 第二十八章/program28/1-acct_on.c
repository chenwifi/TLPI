#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[]){

	if(argc > 2 || (argc > 1 && strcmp(argv[1],"--help") == 0)){
		fprintf(stderr,"usgae: %s filename\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	if(acct(argv[1]) == -1){
		fprintf(stderr,"acct error\n");
		exit(EXIT_FAILURE);
	}

	printf("Process accounting %s\n",(argv[1] == NULL) ? "disabled" : "enabled" );

	return 0;
}
