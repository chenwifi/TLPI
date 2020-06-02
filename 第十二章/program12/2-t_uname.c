#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>

int main(int argc,char *argv[]){
	struct utsname u;

	if(uname(&u) == -1){
		exit(EXIT_FAILURE);
	}

	printf("sysname: %s\n",u.sysname);
	printf("nodename: %s\n",u.nodename);
	printf("release: %s\n",u.release);
	printf("version: %s\n",u.version);
	printf("machine: %s\n",u.machine);
#ifdef _GNU_SOURCE
	printf("domainname: %s\n",u.domainname);
#endif

	return 0;
}
