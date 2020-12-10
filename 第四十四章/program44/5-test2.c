#include <stdio.h>
#include "print_wait_status.h"

int main(){
	FILE *fp;
	int status;

	fp = popen("./5test3","r");
	sleep(10);
	status = pclose(fp);
	printf("	pclose() status == %#x\n",(unsigned int)status);
	if(status != -1){
		printWaitStatus("\t",status);
	}

	return 0;
}
