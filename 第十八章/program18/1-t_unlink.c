#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 200
#define CMD_SIZE 200

int main(int argc,char *argv[]){
	int numSize,fd,i;
	char buf[BUF_SIZE];
	char cmd[CMD_SIZE];

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"Usage: %s filename [blocksize]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	numSize = argc > 2 ? strtol(argv[2],NULL,10) : 100000;
	//printf("%d\n",numSize);
	
	fd = open(argv[1],O_CREAT | O_EXCL | O_WRONLY ,S_IRUSR | S_IWUSR);
	if(fd == -1){
		exit(EXIT_FAILURE);
	}

	unlink(argv[1]);

	for(i = 0;i < numSize;i++){
		write(fd,buf,BUF_SIZE);
	}

	snprintf(cmd,CMD_SIZE,"df -k %s",argv[0]);
	system(cmd);

	close(fd);
	printf("close fd\n");
	
	system(cmd);

	return 0;
}

