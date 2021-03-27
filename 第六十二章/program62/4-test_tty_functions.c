#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

static struct termios userTermios;

void handler(int sig){
	if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&userTermios) != 0){
		fprintf(stderr,"tcsetattr\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

void tstpHandler(int sig){
	struct sigaction sa;
	int savedErrno;
	sigset_t tstpMask,prevMask;
	struct termios ourTermios;

	savedErrno = errno;

	if(tcgetattr(STDIN_FILENO,&ourTermios) == -1){
		fprintf(stderr,"tcgetattr\n");
		exit(EXIT_FAILURE);
	}
	if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&userTermios) == -1){
		fprintf(stderr,"tcsetattr\n");
		exit(EXIT_FAILURE);
	}

	if(sigaction(SIGTSTP,NULL,&sa) != 0){
		fprintf(stderr,"sigaction\n");
		exit(EXIT_FAILURE);
	}

	if(signal(SIGTSTP,SIG_DFL) == SIG_ERR){
		fprintf(stderr,"signal\n");
		exit(EXIT_FAILURE);
	}

	if(raise(SIGTSTP) != 0){
		fprintf(stderr,"raise\n");
		exit(EXIT_FAILURE);
	}

	sigemptyset(&tstpMask);
	sigaddset(&tstpMask,SIGTSTP);
	sigprocmask(SIG_UNBLOCK,&tstpMask,&prevMask);

	//after continue signal
	
	sigprocmask(SIG_SETMASK,&prevMask,NULL);
	if(sigaction(SIGTSTP,&sa,NULL) != 0){
		fprintf(stderr,"sigaction\n");
		exit(EXIT_FAILURE);
	}

	if(tcgetattr(STDIN_FILENO,&userTermios) == -1){
		fprintf(stderr,"tcgetattr\n");
		exit(EXIT_FAILURE);
	}
	if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&ourTermios) == -1){
		fprintf(stderr,"tcsetattr\n");
		exit(EXIT_FAILURE);
	}

	errno = savedErrno;
}

int main(int argc,char *argv[]){
	struct sigaction sa,prev; 
	ssize_t n;
	char ch;

	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if(argc > 1){

		if(tcgetattr(STDIN_FILENO,&userTermios) != 0){
			fprintf(stderr,"tcgetattr\n");
			exit(EXIT_FAILURE);
		}

		sa.sa_handler = handler;
		if(sigaction(SIGQUIT,NULL,&prev) != 0){
			fprintf(stderr,"sigaction\n");
			exit(EXIT_FAILURE);
		}

		if(prev.sa_handler != SIG_IGN){
			if(sigaction(SIGQUIT,&sa,NULL) != 0){
				fprintf(stderr,"sigaction\n");
				exit(EXIT_FAILURE);
			}
		}

		if(sigaction(SIGINT,NULL,&prev) != 0){
			fprintf(stderr,"sigaction\n");
			exit(EXIT_FAILURE);
		}

		if(prev.sa_handler != SIG_IGN){
			if(sigaction(SIGINT,&sa,NULL) != 0){
				fprintf(stderr,"sigaction\n");
				exit(EXIT_FAILURE);
			}
		}

		sa.sa_handler = tstpHandler;
		if(sigaction(SIGTSTP,NULL,&prev) != 0){
			fprintf(stderr,"sigaction\n");
			exit(EXIT_FAILURE);
		}

		if(prev.sa_handler != SIG_IGN){
			if(sigaction(SIGTSTP,&sa,NULL) != 0){
				fprintf(stderr,"sigaction\n");
				exit(EXIT_FAILURE);
			}
		}

		ttySetCbreak(STDIN_FILENO,&userTermios);
	}else{
		ttySetRaw(STDIN_FILENO,&userTermios);
	}

	sa.sa_handler = handler;
	if(sigaction(SIGTERM,&sa,NULL) != 0){
		fprintf(stderr,"sigaction\n");
		exit(EXIT_FAILURE);
	}

	setbuf(stdout,NULL);

	while(1){
		n = read(STDIN_FILENO,&ch,1);
		if(n == -1){
			fprintf(stderr,"read\n");
			break;
		}

		if(n == 0){
			break;
		}

		if(isalpha((unsigned char)ch)){
			putchar(tolower(ch));
		}else if(ch == '\r' || ch == '\n'){
			putchar(ch);
		}else if(iscntrl((unsigned char)ch)){
			printf("^%c",ch ^ 64);
		}else {
			putchar('*');
		}

		if(ch == 'q'){
			break;
		}
	}

	if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&userTermios) == -1){
		fprintf(stderr,"tcsetattr\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
