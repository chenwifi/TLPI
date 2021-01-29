#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAX_LINE 100

int main(int argc,char *argv[]){
	int fd,numRead,cmd,status;
	char line[MAX_LINE];
	char whence,lock,cmdCh;
	struct flock fl;
	long long len,st;

	if(argc != 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s file\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = open(argv[1],O_RDWR);
	if(fd == -1){
		fprintf(stderr,"open\n");
		exit(EXIT_FAILURE);
	}

	printf("Enter ? for help\n");

	while(1){
		printf("PID=%ld> ",(long)getpid());
		fflush(stdout);

		if(fgets(line,MAX_LINE,stdin) == NULL){
			exit(EXIT_SUCCESS);
		}
		line[strlen(line) - 1] = '\0';

		if(*line == '\0'){
			continue;
		}

		whence = 's';
		numRead = sscanf(line,"%c %c %lld %lld %c",&cmdCh,&lock,&st,&len,&whence);
		fl.l_start = st;
		fl.l_len = len;

		if(numRead < 4){
			continue;
		}

		cmd = (cmdCh == 'g') ? F_GETLK : (cmdCh == 's') ? F_SETLK : F_SETLKW;
		fl.l_type = (lock == 'r') ? F_RDLCK : (lock == 'w') ? F_WRLCK : F_UNLCK;
		fl.l_whence = (whence == 'c') ? SEEK_CUR : (whence == 'e') ? SEEK_END : SEEK_SET;

		status = fcntl(fd,cmd,&fl);
		if(cmd == F_GETLK){
			if(status == -1){
				fprintf(stderr,"fcntl-getlk\n");
				exit(EXIT_FAILURE);
			}else{
				if(fl.l_type == F_UNLCK){
					printf("[PID=%ld] lock can be placed\n",(long)getpid());
				}else{
					printf("[PID=%ld] Denied\n",(long)getpid());
				}
			}
		}else{
			if(status == 0){
				printf("[PID=%ld] %s\n",(long)getpid(),(lock == 'u') ? "unlocked" : "got lock");
			}else if(errno == EAGAIN || errno == EACCES){
				printf("[PID=%ld] failed (incompatible lock)\n",(long)getpid());
			}else if(errno == EDEADLK){
				printf("[PID=%ld] failed (dead lock)\n",(long)getpid());
			}else{
				printf("fcntl-F_SETLK\n");
			}
		}
	}

	return 0;
}
