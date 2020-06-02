#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	struct stat sb;

	if(argc != 2){
		fprintf(stderr,"Usage: %s <pathname>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	if(stat(argv[1],&sb) == -1){
		perror("stat");
		exit(EXIT_FAILURE);
	}

	printf("File type:         ");

	switch(sb.st_mode & S_IFMT){
		case S_IFBLK: printf("block device\n"); break;
		case S_IFCHR: printf("character device\n"); break;
		case S_IFDIR: printf("directory\n"); break;
		case S_IFIFO: printf("FIFO/PIPE\n"); break;
		case S_IFLNK: printf("symlink\n"); break;
		case S_IFREG: printf("regular file\n"); break;
		case S_IFSOCK: printf("socket\n"); break;
		default: printf("unknown file");
	}

	printf("I-node NUM: %ld\n",(long)sb.st_ino);
	printf("Mode: %lo(octal)\n",(unsigned long)sb.st_mode);
	printf("Link count: %ld\n",(long)sb.st_nlink);
	printf("Ownership: UID=%ld,GID=%ld\n",(long)sb.st_uid,(long)sb.st_gid);
	printf("Perfect block IO size: %ld bytes\n",(long)sb.st_blksize);
	printf("File size: %lld bytes\n",(long long)sb.st_size);
	printf("Blocks allocated :%lld bytes\n",(long long)sb.st_blocks);
	printf("last status change: %s\n",ctime(&sb.st_ctime));
	printf("last file change: %s\n",ctime(&sb.st_atime));
	printf("last file md: %s\n",ctime(&sb.st_mtime));

	return 0;
}
