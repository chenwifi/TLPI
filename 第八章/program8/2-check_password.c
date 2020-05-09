#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <shadow.h>
#include <stdlib.h>
#include <pwd.h>
#include <string.h>

int main(){
	char *buf;
	int len,blen;
	char *p,*pw,*tmp;
	struct passwd *pwd;
	struct spwd *spd;

	while(1){
		printf("Username: ");
		fflush(stdout);

		len = sysconf(_SC_LOGIN_NAME_MAX);
		buf = (char *)malloc(len);
		fgets(buf,len,stdin);
		/*
		for(p = buf;*p != '\n';p++){
		}
		*p = '\0';
		*/
		blen = strlen(buf);
		if(buf[blen-1] == '\n'){
			buf[blen-1] = '\0';
		}

		//printf("%s\n",buf);
		//exit(0);
	

		pwd = getpwnam(buf);
		if(pwd == NULL){
			exit(EXIT_FAILURE);
		}
		spd = getspnam(buf);
		if(spd != NULL){
			pwd->pw_passwd = spd->sp_pwdp;
		}

		p = getpass("Password: ");
		pw = crypt(p,pwd->pw_passwd);
		for(tmp = p;*tmp != '\0';tmp++){
			*tmp = '\0';
		}

		if(strcmp(pw,pwd->pw_passwd) == 0){
			printf("auth ok!!!\n");
			exit(EXIT_SUCCESS);
		}
	}

	return 0;
}
