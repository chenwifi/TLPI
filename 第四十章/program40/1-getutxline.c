#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <utmpx.h>
#include <time.h>

int main(int argc,char *argv[]){
	struct utmpx *ut;
	struct utmpx uth;

	if(argc > 1 && strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s [utmp-file]\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	if(argc > 1){
		if(utmpxname(argv[1]) == -1){
			fprintf(stderr,"utmpxname\n");
			exit(EXIT_FAILURE);
		}
	}

	setutxent();

	uth.ut_type = USER_PROCESS;
	strcpy(uth.ut_id, "tty1");
	printf("%s\n",uth.ut_id);
	//strcpy(uth.ut_id,"1");

	printf("user	type		PID	line	id	host	date/time\n");

	while((ut = getutxid(&uth)) != NULL){
		printf("%-8s ",ut->ut_user);
		printf("%-9.9s ",
				(ut->ut_type == EMPTY) ? "EMPTY" :
				(ut->ut_type == RUN_LVL) ? "RUN_LVL" :
				(ut->ut_type == BOOT_TIME) ? "BOOT_TIME" :
				(ut->ut_type == NEW_TIME) ? "NEW_TIME" :
				(ut->ut_type == OLD_TIME) ? "OLD_TIME" :
				(ut->ut_type == INIT_PROCESS) ? "INIT_PROCESS" :
				(ut->ut_type == LOGIN_PROCESS) ? "LOGIN_PROCESS" :
				(ut->ut_type == USER_PROCESS) ? "USER_PROCESS" :
				(ut->ut_type == DEAD_PROCESS) ? "DEAD_PROCESS" : "???"
				);

		printf("%5ld %-6.6s %-3.5s %-9.9s ",(long)ut->ut_pid,ut->ut_line,ut->ut_id,ut->ut_host);
		printf("%s",ctime((time_t *) &(ut->ut_tv.tv_sec)));
	}

	endutxent();

	return 0;
}
