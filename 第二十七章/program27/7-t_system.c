#include <stdio.h>
#include <stdlib.h>
#include "print_wait_status.h"

#define LEN 200

int main(int argc,char *argv[]){
	char str[LEN];
	int status;

	while(1){
		printf("Command: ");
		fflush(stdout);
		if(fgets(str,LEN,stdin) == NULL){
			break;
		}
		status = system(str);

		printf("system return: status=0x%04x(%d,%d)\n",status,status >> 8,status & 0x0f);
		
		if(status == -1){
			fprintf(stdout,"system error\n");
			exit(EXIT_SUCCESS);
		}else{
			if(WIFEXITED(status) && WEXITSTATUS(status) == 127){
				printf("probably could not invoke shell\n");
			}else{
				printWaitStatus(NULL,status);
			}
		}
	}

	return 0;
}
