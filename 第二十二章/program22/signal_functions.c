#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "signal_functions.h"

/*
int main(){
	printf("%d\n",NSIG);

	return 0;
}
*/


void printSigset(FILE *of,const char *prefix,const sigset_t *sigset){
	int i,cnt = 0;

	for(i = 1;i < NSIG;i++){
		if(sigismember(sigset,i)){
			cnt++;
			fprintf(of,"%s%d (%s)\n",prefix,i,strsignal(i));
		}
	}

	if(cnt == 0){
		fprintf(of,"%s<empty signal set>\n",prefix);
	}
}

int printSigMask(FILE *of,const char *msg){
	sigset_t pre;

	if(msg != NULL){
		fprintf(of,"%s\n",msg);
	}

	sigprocmask(SIG_BLOCK,NULL,&pre);
	printSigset(of,"\t\t",&pre);

	return 0;
}

int printPendingSigs(FILE *of,const char *msg){
	sigset_t pending;

	if(msg != NULL){
		fprintf(of,"%s\n",msg);
	}

	sigpending(&pending);
	printSigset(of,"\t\t",&pending);

	return 0;
}
