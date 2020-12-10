#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc,char *argv[]){
	int i;

	for(i = 1; i < argc; i++){
		if(shmctl(atoi(argv[i]),IPC_RMID,NULL) == -1){
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
