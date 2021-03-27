#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define BUF_SIZE 100

int main(int argc,char *argv[]){
	struct termios tp,save;
	char buf[BUF_SIZE];

	if(tcgetattr(STDIN_FILENO,&tp) != 0){
		fprintf(stderr,"tcgetattr\n");
		exit(EXIT_FAILURE);
	}

	save = tp;

	tp.c_lflag &= ~ECHO;

	if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&tp) != 0){
		fprintf(stderr,"tcsetattr\n");
		exit(EXIT_FAILURE);
	}

	printf("Enter text: ");
	fflush(stdout);

	if(fgets(buf,BUF_SIZE,stdin) == NULL){
		printf("Got end of file or error on fgets\n");
	}else{
		printf("\nRead: %s",buf);
	}

	if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&save) != 0){
		fprintf(stderr,"tcsetattr\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
