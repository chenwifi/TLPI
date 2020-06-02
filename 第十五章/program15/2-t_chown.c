#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

gid_t groupIdFromName(const char *name){
	struct group *gp;
	gid_t g;
	char *endptr;

	if(name == NULL || *name == '\0'){
		return -1;
	}
	g = strtol(name,&endptr,10);
	if(*endptr == '\0'){
		return g;
	}

	gp = getgrnam(name);
	if(gp == NULL){
		return -1;
	}

	return gp->gr_gid;
}

uid_t userIdFromName(const char *name){
	struct passwd *pd;
	uid_t u;
	char *endptr;

	if(name == NULL || *name == '\0'){
		return -1;
	}

	u = strtol(name,&endptr,10);
	if(*endptr == '\0'){
		return u;
	}

	pd = getpwnam(name);
	if(pd == NULL){
		return -1;
	}

	return pd->pw_uid;
}

int main(int argc,char *argv[]){
	uid_t u;
	gid_t g;
	int i,flag;

	if(argc < 4 || (strcmp(argv[1],"--help") == 0)){
		fprintf(stderr,"Usage: %s uname gname filename\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	if(strcmp(argv[1], "-") == 0){
		u = -1;
	}else{
		u = userIdFromName(argv[1]);
	}

	if(strcmp(argv[2], "-") == 0){
		g = -1;
	}else{
		g = groupIdFromName(argv[2]);
	}

	flag = 0;
	for(i = 3; i < argc;i++){
		if(chown(argv[i],u,g) == -1){
			fprintf(stderr,"chown error\n");
			flag = 1;
		}
	}

	exit(flag ? EXIT_SUCCESS : EXIT_FAILURE);
}
