#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MYFILE "myfile"
#define MYDIR "mydir"

#define FILE_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define DIR_PERMS (S_IRWXU | S_IRWXG | S_IRWXO)
#define UMASK_SETTINGS (S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH)

int main(){
	mode_t u;
	int fd,d;
	struct stat buf;

	u = umask(UMASK_SETTINGS);

	fd = open(MYFILE,O_CREAT | O_RDWR | O_EXCL,FILE_PERMS);
	if(fd == -1){
		exit(EXIT_FAILURE);
	}
	stat(MYFILE,&buf);
	printf("request : %.3o;umask : %.3o ;end : %.3o;\n",FILE_PERMS,UMASK_SETTINGS,buf.st_mode);

	unlink(MYFILE);

	d = mkdir(MYDIR,DIR_PERMS);
	if(d == -1){
		exit(EXIT_FAILURE);
	}
	stat(MYDIR,&buf);
	printf("request : %.3o;umask : %.3o ;end : %.3o;\n",DIR_PERMS,UMASK_SETTINGS,buf.st_mode);
	rmdir(MYDIR);

	return 0;
}
