#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_once_t once = PTHREAD_ONCE_INIT;
pthread_key_t strerrorKey;

#define MAX_ERROR_LEN 256

int glob = 0;

static void destructor(void *buf){
	free(buf);
	glob++;
	printf("glob=%d\n",glob);
}

static void createKey(void){
	int s;

	s = pthread_key_create(&strerrorKey,destructor);
	if(s != 0){
		fprintf(stderr,"pthread_key_create\n");
		exit(EXIT_FAILURE);
	}
}

char *mystrerror(int err){
	int s;
	char *buf;

	s = pthread_once(&once,createKey);
	if(s != 0){
		fprintf(stderr,"pthread_once\n");
		exit(EXIT_FAILURE);
	}

	if((buf = pthread_getspecific(strerrorKey)) == NULL){
		buf = (char *)malloc(MAX_ERROR_LEN);
		if(buf == NULL){
			fprintf(stderr,"malloc");
			exit(EXIT_FAILURE);
		}

		printf("in malloc\n");

		s = pthread_setspecific(strerrorKey,(void *)buf);
		if(s != 0){
			fprintf(stderr,"pthread_setspecific\n");
			exit(EXIT_FAILURE);
		}
	}

	if(err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL){
		snprintf(buf,MAX_ERROR_LEN,"Unknown error %d",err);
	}else{
		strncpy(buf,_sys_errlist[err],MAX_ERROR_LEN - 1);
		buf[MAX_ERROR_LEN - 1] = '\0';
	}

	return buf;
}

