#include <stdio.h>
#include <syslog.h>

int main(int argc,char *argv[]){

	syslog(LOG_EMERG,"hello world");

	return 0;
}
