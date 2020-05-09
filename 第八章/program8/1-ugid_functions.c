#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>

char * userNameFromId(uid_t uid){
	struct passwd *pwd;

	pwd = getpwuid(uid);
	return (pwd==NULL) ? NULL : pwd->pw_name;
}

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

char * groupNameFromId(gid_t gid){
	struct group *grp;

	grp = getgrgid(gid);

	return (grp == NULL) ? NULL : grp->gr_name;
}

gid_t groupIdFromName(const char *name){
	struct group *grp;
	gid_t g;
	char *endptr;

	if(name == NULL || *name == '\0'){
		return -1;
	}

	g = strtol(name,&endptr,10);
	if(*endptr == '\0'){
		return g;
	}

	grp = getgrnam(name);
	
	return (grp == NULL) ? -1 : grp->gr_gid;
}

int main(int argc,char *argv[]){
	char *name;
	uid_t uid;
	gid_t gid;

	switch (atoi(argv[1])){
		case 1:
			name = userNameFromId(atoi(argv[2]));
			printf("%s\n",name);
			break;
		case 2:
			uid = userIdFromName(argv[2]);
			printf("%ld\n",uid);
			break;
		case 3:
			name = groupNameFromId(atoi(argv[2]));
			printf("%s\n",name);
			break;
		default:
			gid = groupIdFromName(argv[2]);
			printf("%ld\n",gid);
			break;
	}

	return 0;
}
