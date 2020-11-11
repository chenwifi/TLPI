#include <stdio.h>
#include <paths.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <utmpx.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]){
	struct utmpx ut;
	char *devName;

	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s username [sleep-time]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	memset(&ut,0,sizeof(struct utmpx));
	ut.ut_type = USER_PROCESS;
	strncpy(ut.ut_user,argv[1],sizeof(ut.ut_user));
	if(time((time_t *)&ut.ut_tv.tv_sec) == -1){
		fprintf(stderr,"time\n");
		exit(EXIT_FAILURE);
	}
	ut.ut_pid = getpid();

	devName = ttyname(STDIN_FILENO);
	//printf("%s\n",devName);
	if(devName == NULL){
		fprintf(stderr,"ttyname\n");
		exit(EXIT_FAILURE);
	}

	strncpy(ut.ut_line,devName + 5,sizeof(ut.ut_line));
	strncpy(ut.ut_id,devName + 9,sizeof(ut.ut_id));

	printf("Creating login entries in utmp and wtmp\n");
	printf("		using pid %ld,line %.*s,id %.*s\n",(long)ut.ut_pid,(int)sizeof(ut.ut_line),ut.ut_line,(int)sizeof(ut.ut_id),ut.ut_id);

	setutxent();
	if(pututxline(&ut) == NULL){
		fprintf(stderr,"pututxline\n");
		exit(EXIT_FAILURE);
	}

	updwtmpx(_PATH_WTMP,&ut);

	sleep(15);

	ut.ut_type = DEAD_PROCESS;
	if(time((time_t *)&ut.ut_tv.tv_sec) == -1){
		fprintf(stderr,"time\n");
		exit(EXIT_FAILURE);
	}
	memset(&ut.ut_user,0,sizeof(ut.ut_user));

	printf("creating logout entries in utmp and wtmp\n");

	setutxent();
	if(pututxline(&ut) == NULL){
		fprintf(stderr,"pututxline\n");
		exit(EXIT_FAILURE);
	}

	updwtmpx(_PATH_WTMP,&ut);

	endutxent();

	return 0;
}
