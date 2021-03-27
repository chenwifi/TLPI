#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

static int pfd[2];

static void handler(int sig){
	int savedErrno;

	savedErrno = errno;

	if(write(pfd[1],"x",1) != 1 && errno != EAGAIN){
		fprintf(stderr,"write\n");
		exit(EXIT_FAILURE);
	}

	errno = savedErrno;
}

int main(int argc,char *argv[]){
	fd_set rset;
	int nfds,flags,ready;
	struct timeval tv;
	struct timeval *tvp;
	struct sigaction sa;
	char buf;

	if(pipe(pfd) == -1){
		fprintf(stderr,"pipe\n");
		exit(EXIT_FAILURE);
	}

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	tvp = &tv;

	nfds = 0;
	FD_ZERO(&rset);
	FD_SET(STDIN_FILENO,&rset);
	FD_SET(pfd[0],&rset);
	nfds = nfds > STDIN_FILENO + 1 ? nfds : STDIN_FILENO + 1;
	nfds = nfds > pfd[0] + 1 ? nfds : pfd[0] + 1;

	flags = fcntl(pfd[0],F_GETFL);
	if(flags == -1){
		fprintf(stderr,"fcntl\n");
		exit(EXIT_FAILURE);
	}
	flags |= O_NONBLOCK;
	if(fcntl(pfd[0],F_SETFL,flags) == -1){
		fprintf(stderr,"fcntl\n");
		exit(EXIT_FAILURE);
	}

	flags = fcntl(pfd[1],F_GETFL);
	if(flags == -1){
		fprintf(stderr,"fcntl\n");
		exit(EXIT_FAILURE);
	}
	flags |= O_NONBLOCK;
	if(fcntl(pfd[1],F_SETFL,flags) == -1){
		fprintf(stderr,"fcntl\n");
		exit(EXIT_FAILURE);
	}

	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if(sigaction(SIGINT,&sa,NULL) == -1){
		fprintf(stderr,"sigaction\n");
		exit(EXIT_FAILURE);
	}

	sleep(10);

	while((ready = select(nfds,&rset,NULL,NULL,tvp)) == -1 && errno == EINTR){
		continue;
	}

	if(ready == -1){
		fprintf(stderr,"select\n");
		exit(EXIT_FAILURE);
	}

	if(FD_ISSET(pfd[0],&rset)){
		printf("A signal was got\n");

		while(1){
			if(read(pfd[0],&buf,1) == -1){
				if(errno == EAGAIN){
					break;
				}else{
					fprintf(stderr,"read\n");
					exit(EXIT_FAILURE);
				}
			}
		}
	}

	return 0;
}
