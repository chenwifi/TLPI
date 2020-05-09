#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <errno.h>

int main(int argc,char *argv[]){
	struct passwd pwd;
	struct passwd *result;
	char *buf;
	size_t buflen;
	int s;

	if(argc != 2){
		fprintf(stderr,"%s username\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	buflen = sysconf(_SC_GETPW_R_SIZE_MAX);
	if(buflen == -1){
		buflen = 16384;
	}
	buf = (char *)malloc(buflen);
	if(buf == NULL){
		exit(EXIT_FAILURE);
	}

	s = getpwnam_r(argv[1],&pwd,buf,buflen,&result);
	if(result == NULL){
		if(s == 0){
			fprintf(stderr,"not found\n");
		}else{
			errno = s;
			perror("getpwnam_r");
		}
		exit(EXIT_FAILURE);
	}

	printf("username: %s . userid : %d\n",result->pw_name,result->pw_uid);
	printf("%s\n",buf);
	write(1,buf,buflen);
	
	return 0;
}
