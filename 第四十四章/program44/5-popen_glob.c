#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "print_wait_status.h"

#define PAT_SIZE 50
#define POPEN_FMT "/bin/ls -d %s 2> /dev/null"
#define PCMD_BUF_SIZE (sizeof(POPEN_FMT) + PAT_SIZE)

int main(int argc,char *argv[]){
	char pat[PAT_SIZE];
	int len,i,badPattern,fileCnt,status;
	char popenCmd[PCMD_BUF_SIZE];
	FILE *fp;
	char pathname[100];

	while(1){
		printf("pattern: ");
		fflush(stdout);
		if(fgets(pat,PAT_SIZE,stdin) == NULL){
			break;
		}
		len = strlen(pat);
		if(len <= 1){
			continue;
		}
		if(pat[len - 1] == '\n'){
			pat[len - 1] = '\0';
		}

		for(i = 0,badPattern = 0; i < len && !badPattern; i++){
			if(!isalnum(pat[i]) && strchr("_*?[^-]",pat[i]) == NULL){
				badPattern = 1;
			}
		}

		if(badPattern){
			printf("Bad pattern character:%c\n",pat[i - 1]);
			continue;
		}

		snprintf(popenCmd,PCMD_BUF_SIZE,POPEN_FMT,pat);
		popenCmd[PCMD_BUF_SIZE - 1] = '\0';

		fp = popen(popenCmd,"r");
		if(fp == NULL){
			printf("popen fail\n");
			continue;
		}

		fileCnt = 0;
		while(fgets(pathname,100,fp) != NULL){
			printf("%s",pathname);
			fileCnt++;
		}

		status = pclose(fp);
		printf("	%d matching file\n",fileCnt);
		printf("	pclose() status == %#x\n",(unsigned int)status);
		if(status != -1){
			printWaitStatus("\t",status);
		}
	}

	return 0;
}
