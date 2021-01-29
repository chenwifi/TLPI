#include <stdio.h>
#include <unistd.h>
#include "4-create_pid_file.h"

int main(int argc,char *argv[]){
	int fd;

	fd = createPidFile(argv[0],"./test.pid",CPF_CLOEXEC);

	printf("successfully\n");
	sleep(60);

	return 0;
}
