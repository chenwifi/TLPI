#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 10

int main(int argc,char *argv[]){
	int pfd[2];
	char buf[BUF_SIZE];
	ssize_t len;

	if(argc != 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s string\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	if(pipe(pfd) == -1){
		fprintf(stderr,"pipe\n");
		exit(EXIT_FAILURE);
	}

	switch(fork()){
		case -1:
			fprintf(stderr,"fork\n");
			exit(EXIT_FAILURE);
			break;
		case 0:
			close(pfd[1]);
			while(1){
				len = read(pfd[0],buf,BUF_SIZE);
				if(len == -1){
					_exit(EXIT_FAILURE);
				}

				if(len == 0){
					break;
				}

				if(len > 0){
					write(STDOUT_FILENO,buf,len);
				}
			}

			write(STDOUT_FILENO,"\n",1);
			close(pfd[0]);
			_exit(EXIT_SUCCESS);
			break;
		default:
			close(pfd[0]);
			write(pfd[1],argv[1],strlen(argv[1]));
			close(pfd[1]);
			wait(NULL);
			break;
	}

	return 0;
}
