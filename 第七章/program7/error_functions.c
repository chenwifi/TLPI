#include <stdarg.h>
#include "error_functions.h"
#include "tlpi_hdr.h"

/*
#ifdef __GNUC__
__attribute__((__noreturn__))
#endif
*/

static void outputError(Boolean useErr,int err,Boolean flushStdout,const char *format,va_list ap){
#define BUF_SIZE 500
	char buf[BUF_SIZE],userMsg[BUF_SIZE],errText[BUF_SIZE];

	vsnprintf(userMsg,BUF_SIZE,format,ap);

	if(useErr){
		snprintf(errText,BUF_SIZE,"[%s]",strerror(err));
	}else{
		snprintf(errText,BUF_SIZE,":");
	}

	snprintf(buf,BUF_SIZE,"ERROR%s %s\n",errText,userMsg);

	if(flushStdout)
		fflush(stdout);

	fputs(buf,stderr);
	fflush(stderr);
}


#ifdef __GNUC__
__attribute__((__noreturn__))
#endif
static void terminate(Boolean useExit3){
	char *s;

	s = getenv("EF_DUMPCORE");
	if(s != NULL && *s != '\0'){
		abort();
	}else if(useExit3){
		exit(EXIT_FAILURE);
	}else{
		_exit(EXIT_FAILURE);
	}
}

void usageErr(const char *format,...){
	va_list argList;

	fflush(stdout);

	fprintf(stderr,"Usage: ");
	va_start(argList,format);
	vfprintf(stderr,format,argList);
	va_end(argList);

	fflush(stderr);
	exit(EXIT_FAILURE);
}

void errExit(const char *format,...){
	va_list argList;

	va_start(argList,format);
	outputError(TRUE,errno,TRUE,format,argList);
	va_end(argList);

	terminate(TRUE);
}

void fatal(const char *format,...){
	va_list argList;

	va_start(argList,format);
	outputError(FALSE,0,TRUE,format,argList);
	va_end(argList);

	terminate(TRUE);
}

void cmdLineErr(const char *format,...){
	va_list argList;

	fflush(stdout);

	fprintf(stderr,"Command-line usage error: ");
	va_start(argList,format);
	vfprintf(stderr,format,argList);
	va_end(argList);

	fflush(stderr);
	exit(EXIT_FAILURE);
}
