#define _GNU_SOURCE
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

#define SG_SIZE (NGROUPS_MAX + 1)

char * userNameFromId(uid_t uid){
	struct passwd *pwd;

	pwd = getpwuid(uid);
	return (pwd==NULL) ? NULL : pwd->pw_name;
}

char * groupNameFromId(gid_t gid){
	struct group *grp;

	grp = getgrgid(gid);

	return (grp == NULL) ? NULL : grp->gr_name;
}

int main(){
	uid_t ruid,euid,suid,fsuid;
	gid_t rgid,egid,sgid,fsgid;
	char *p;
	int j,numGroups;
	gid_t suppGroups[SG_SIZE];

	if(getresuid(&ruid,&euid,&suid) == -1){
		exit(EXIT_FAILURE);
	}

	if(getresgid(&rgid,&egid,&sgid) == -1){
		exit(EXIT_FAILURE);
	}

	fsuid = setfsuid(0);
	fsgid = setfsgid(0);

	printf("UID: ");
	p = userNameFromId(ruid);
	printf("real=%s (%ld); ",(p == NULL) ? "???" : p,(long)ruid);
	p = userNameFromId(euid);
	printf("eff=%s (%ld); ",(p == NULL) ? "???" : p,(long)euid);
	p = userNameFromId(suid);
	printf("saved=%s (%ld); ",(p == NULL) ? "???" : p,(long)suid);
	p = userNameFromId(fsuid);
	printf("fs=%s (%ld); ",(p == NULL) ? "???" : p,(long)fsuid);
	printf("\n");

	printf("GID: ");
	p = groupNameFromId(rgid);
	printf("real=%s (%ld); ",(p == NULL) ? "???" : p,(long)rgid);
	p = groupNameFromId(egid);
	printf("eff=%s (%ld); ",(p == NULL) ? "???" : p,(long)egid);
	p = groupNameFromId(sgid);
	printf("saved=%s (%ld); ",(p == NULL) ? "???" : p,(long)sgid);
	p = groupNameFromId(fsgid);
	printf("fs=%s (%ld); ",(p == NULL) ? "???" : p,(long)fsgid);
	printf("\n");

	numGroups = getgroups(SG_SIZE,suppGroups);
	if(numGroups == -1){
		exit(EXIT_FAILURE);
	}
	for(j = 0;j < numGroups;j++){
		p = groupNameFromId(suppGroups[j]);
		printf("%s (%ld); ",(p == NULL) ? "???" : p,(long)suppGroups[j]);
	}
	printf("\n");

	return 0;
}
