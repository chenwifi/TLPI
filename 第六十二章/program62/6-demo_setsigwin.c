#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

int main(int argc,char *argv[]){
	struct winsize ws;

	ws.ws_row = 26;
	ws.ws_col = 76;
	if(ioctl(STDIN_FILENO,TIOCSWINSZ,&ws) == -1){
		fprintf(stderr,"ioctl\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
