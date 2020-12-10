#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEMOPS 1000

static int parseOps(char *arg,struct sembuf *sops){
	int numOps;
	char *remaining,*sign,*flag,*comma;

	for(numOps = 0,remaining = arg;; numOps++){
		if(numOps >= MAX_SEMOPS){
			fprintf(stderr,"too many op\n");
			exit(EXIT_FAILURE);
		}

		if(*remaining == '\0'){
			fprintf(stderr,"empty string\n");
			exit(EXIT_FAILURE);
		}

		if(!isdigit((unsigned char )*remaining)){
			fprintf(stderr,"except digit init\n");
			exit(EXIT_FAILURE);
		}

		sops[numOps].sem_num = strtol(remaining,&sign,10);
		if(*sign == '\0' || strchr("-=+",*sign) == NULL){
			fprintf(stderr,"except - = +\n");
			exit(EXIT_FAILURE);
		}
		if(!isdigit((unsigned char )*(sign + 1))){
			fprintf(stderr,"except digit init\n");
			exit(EXIT_FAILURE);
		}

		sops[numOps].sem_op = strtol(sign + 1,&flag,10);
		if(*sign == '-'){
			sops[numOps].sem_op = -sops[numOps].sem_op;
		}else if(*sign == '='){
			if(sops[numOps].sem_op != 0){
				exit(EXIT_FAILURE);
			}
		}

		sops[numOps].sem_flg = 0;
		for(;;flag++){
			if(*flag == 'n'){
				sops[numOps].sem_flg |= IPC_NOWAIT;
			}else if(*flag == 'u'){
				sops[numOps].sem_flg |= SEM_UNDO;
			}else{
				break;
			}
		}

		if(*flag != ',' && *flag != '\0'){
			exit(EXIT_FAILURE);
		}

		comma = strchr(remaining,',');
		if(comma == NULL){
			break;
		}else{
			remaining = comma + 1;
		}
	}

	return numOps + 1;
}

int main(int argc,char *argv[]){
	int i,nsops;
	struct sembuf sops[MAX_SEMOPS];

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		exit(EXIT_FAILURE);
	}

	for(i = 2; i < argc; i++){
		nsops = parseOps(argv[i],sops);

		printf("%ld: about to semop() [%s]\n",(long)getpid(),argv[i]);

		if(semop(atoi(argv[1]),sops,nsops) == -1){
			fprintf(stderr,"semop\n");
			exit(EXIT_FAILURE);
		}

		printf("%ld: semop() complete [%s]\n",(long)getpid(),argv[i]);
	}

	return 0;
}
