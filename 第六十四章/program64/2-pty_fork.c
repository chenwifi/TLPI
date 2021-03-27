#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "1-pty_master_open.h"

#define MAX_SNAME 1000

pid_t ptyFork(int *masterFd,char *slaveName,size_t snLen,const struct termios *slaveTermios,const struct winsize *slaveWS){
	int mfd,savedErrno,slaveFd;
	pid_t childPid;
	char slname[MAX_SNAME];

	mfd = ptyMasterOpen(slname,MAX_SNAME);
	if(mfd == -1){
		return -1;
	}

	if(slaveName != NULL){
		if(strlen(slname) < snLen){
			strncpy(slaveName,slname,snLen);
		}else{
			close(mfd);
			errno = EOVERFLOW;
			return -1;
		}
	}

	childPid = fork();
	if(childPid == -1){
		savedErrno = errno;
		close(mfd);
		errno = savedErrno;

		return -1;
	}

	if(childPid != 0){
		*masterFd = mfd;
		return childPid;
	}

	if(setsid() == -1){
		fprintf(stderr,"setsid\n");
		exit(EXIT_FAILURE);
	}

	close(mfd);

	slaveFd = open(slname,O_RDWR);
	if(slaveFd == -1){
		fprintf(stderr,"open\n");
		exit(EXIT_FAILURE);
	}

#ifdef TIOCSCTTY
	if(ioctl(slaveFd,TIOCSCTTY,0) == -1){
		fprintf(stderr,"ioctl\n");
		exit(EXIT_FAILURE);
	}
#endif

	if(slaveTermios != NULL){
		if(tcsetattr(slaveFd,TCSANOW,slaveTermios) == -1){
			fprintf(stderr,"tcsetattr\n");
			exit(EXIT_FAILURE);
		}
	}

	if(slaveWS != NULL){
		if(ioctl(slaveFd,TIOCSWINSZ,slaveWS) == -1){
			fprintf(stderr,"ioctl\n");
			exit(EXIT_FAILURE);
		}
	}

	if(dup2(slaveFd,STDIN_FILENO) != STDIN_FILENO){
		fprintf(stderr,"dup2\n");
		exit(EXIT_FAILURE);
	}
	if(dup2(slaveFd,STDOUT_FILENO) != STDOUT_FILENO){
		fprintf(stderr,"dup2\n");
		exit(EXIT_FAILURE);
	}
	if(dup2(slaveFd,STDERR_FILENO) != STDERR_FILENO){
		fprintf(stderr,"dup2\n");
		exit(EXIT_FAILURE);
	}

	if(slaveFd > STDERR_FILENO){
		close(slaveFd);
	}

	return 0;
}
