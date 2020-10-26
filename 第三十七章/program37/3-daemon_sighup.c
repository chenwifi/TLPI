#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "1-become_daemon.h"

static const char *LOG_FILE = "/tmp/ds.log";
static const char *CONFIG_FILE = "/tmp/ds.conf";

static FILE *logfp;

static volatile sig_atomic_t hup = 0;

static void logMessage(const char *format, ...){
	va_list argList;
	const char *T_FMT = "%F %X";
#define T_BUF_SIZE sizeof("YYYY-MM-DD HH:MM:SS")
	char timestamp[T_BUF_SIZE];
	time_t t;
	struct tm *loc;

	t = time(NULL);
	loc = localtime(&t);
	if(loc == NULL || strftime(timestamp,T_BUF_SIZE,T_FMT,loc) == 0){
		fprintf(logfp,"???UNKNOWN TIME???: ");
	}else{
		fprintf(logfp,"%s: ",timestamp);
	}

	va_start(argList,format);
	vfprintf(logfp,format,argList);
	fprintf(logfp,"\n");
	va_end(argList);
}

static void logOpen(const char *logFilename){
	mode_t m;
	m = umask(077);
	logfp = fopen(logFilename,"a");
	umask(m);

	if(logfp == NULL){
		exit(EXIT_FAILURE);
	}

	setbuf(logfp,NULL);

	logMessage("open log file");
}

static void logClose(){
	logMessage("closing log file");
	fclose(logfp);
}

static void readConfigFile(const char *configFilename){
	FILE *configfp;
	char str[100];

	configfp = fopen(configFilename,"r");
	if(configfp != NULL){
		if(fgets(str,100,configfp) == NULL){
			str[0] = '\0';
		}else{
			str[strlen(str) - 1] = '\0';
		}

		logMessage("Read config file: %s",str);
		fclose(configfp);
	}
}

static void handler(int sig){
	hup = 1;
}

int main(int argc,char *argv[]){
	const int SLEEP_TIME = 15;
	struct sigaction sa;
	int unslept;
	int count = 0;

	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sigaction(SIGHUP,&sa,NULL);

	if(becomeDaemon(0) == -1){
		fprintf(stderr,"becomeDaemon error\n");
		exit(EXIT_FAILURE);
	}

	logOpen(LOG_FILE);
	readConfigFile(CONFIG_FILE);

	unslept = SLEEP_TIME;

	while(1){
		unslept = sleep(unslept);

		if(hup){
			hup = 0;
			logClose();
			logOpen(LOG_FILE);
			readConfigFile(CONFIG_FILE);
		}

		if(unslept == 0){
			count++;
			logMessage("Main: %d",count);
			unslept = SLEEP_TIME;
		}
	}

	return 0;
}
