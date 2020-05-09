#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BUF_SIZE 1000

char *currTime(const char *format){
	struct tm *timeptr;
	time_t t;
	static char buf[BUF_SIZE];
	size_t s;

	t = time(NULL);
	timeptr = localtime(&t);
	if(timeptr == NULL){
		return NULL;
	}
	s = strftime(buf,BUF_SIZE,((format == NULL ) ? "%c" : format),timeptr);
	if(s == 0){
		return NULL;
	}

	return buf;
}

int main(){
	char *s;

	s = currTime(NULL);
	printf("%s\n",s);

	return 0;
}
