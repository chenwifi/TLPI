#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <lastlog.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <paths.h>

uid_t userIdFromName(const char *name){
	struct passwd *pwd;
	uid_t u;
	char *endptr;

	if(name == NULL || *name == '\0'){
		return -1;
	}

	u = strtol(name,&endptr,10);
	if(*endptr == '\0'){
		return u;
	}

	pwd = getpwnam(name);
	if(pwd == NULL){
		return -1;
	}
	return pwd->pw_uid; 
}

int main(int argc,char *argv[]){
	int fd,i;
	uid_t uid;
	struct lastlog llog;

	fd = open(_PATH_LASTLOG,O_RDONLY);

	for(i = 1; i < argc; i++){
		uid = userIdFromName(argv[i]);
		if(uid == -1){
			fprintf(stderr,"no such user: %s\n",argv[i]);
		}

		lseek(fd,uid * sizeof(struct lastlog),SEEK_SET);
		read(fd,&llog,sizeof(struct lastlog));

		printf("%-8.8s %-6.6s %-20.20s %s\n",argv[i],llog.ll_line,llog.ll_host,ctime((time_t *)&llog.ll_time));
	}

	return 0;
}
