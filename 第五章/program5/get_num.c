#include <stdio.h>
#include "get_num.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static void getFail(const char *fname,const char *msg,const char *arg,const char *name){
	fprintf(stderr,"%s error",fname);

	if(name != NULL){
		fprintf(stderr," (in %s)",name);
	}
	fprintf(stderr,": %s\n",msg);
	
	if(arg != NULL && *arg != '\0'){
		fprintf(stderr,"   offending text: %s\n",arg);
	}

	exit(EXIT_FAILURE);
}

static long getNum(const char *fname,const char *arg,int flags,const char *name){
	int base;
	char *endptr;
	long res;

	if(arg == NULL || *arg == '\0'){
		getFail(fname,"null or empty string",arg,name);
	}

	errno = 0;
	base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 : (flags & GN_BASE_16) ? 16 : 10;
	res = strtol(arg,&endptr,base);
	if(errno != 0){
		getFail(fname,"strtol() fail",arg,name);
	}

	if(*endptr != '\0'){
		getFail(fname,"nonnumberic characters",arg,name);
	}

	if((flags & GN_NONNEG) && res < 0){
		getFail(fname,"negative value is not allowed",arg,name);
	}

	if((flags & GN_GT_0) && res <= 0){
		getFail(fname,"value must be > 0",arg,name);
	}

	return res;
}

long getLong(const char *arg,int flags,const char *name){
	return getNum("getLong",arg,flags,name);
}

long getInt(const char *arg,int flags,const char *name){
	long res;

	res = getNum("getInt",arg,flags,name);
	if(res > INT_MAX || res < INT_MIN){
		getFail("getInt","integer out of range",arg,name);
	}

	return (int)res;
}
