#include <stdio.h>
#include <libgen.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define CMD_SIZE 200

int main(int argc,char *argv[]){
	pid_t childPid;
	char cmd[CMD_SIZE];

	setbuf(stdout,NULL);

	printf("Parent PID=%ld\n",(long)getpid());

	switch(childPid = fork()){
		case -1:
			fprintf(stdout,"fork error\n");
			exit(EXIT_FAILURE);
			break;
		case 0:
			printf("CHILD (%ld) exiting\n",(long)getpid());
			_exit(EXIT_FAILURE);
			break;
		default:
			sleep(3);
			snprintf(cmd,CMD_SIZE,"ps | grep %s",basename(argv[0]));
			cmd[CMD_SIZE - 1] = '\0';
			system(cmd);
			kill(childPid,SIGKILL);
			sleep(3);
			printf("After sending SIGKILL to zombie (%ld)\n",(long)childPid);
			system(cmd);
			break;
	}

	return 0;
}
