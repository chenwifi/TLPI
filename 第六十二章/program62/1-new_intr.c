#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

int main(int argc,char *argv[]){
	int intrChar;
	struct termios tp;

	if(argc > 1 && strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s [intr-char]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	if(argc == 1){
		intrChar = fpathconf(STDIN_FILENO,_PC_VDISABLE);
		if(intrChar == -1){
			fprintf(stderr,"could not get disable\n");
			exit(EXIT_FAILURE);
		}
	}else if(isdigit(argv[1][0])){
		intrChar = strtoul(argv[1],NULL,0);
	}else{
		intrChar = argv[1][0];
	}

	if(tcgetattr(STDIN_FILENO,&tp) == -1){
		fprintf(stderr,"tcgetattr\n");
		exit(EXIT_FAILURE);
	}

	tp.c_cc[VINTR] = intrChar;

	if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&tp) == -1){
		fprintf(stderr,"tcsetattr\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
