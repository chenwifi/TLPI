#include <stdio.h>

extern char **environ;

int main(int argc,char *argv[]){
	int i;
	char **env;

	for(i = 0; i < argc; i++){
		printf("argv[%d] = %s\n",i,argv[i]);
	}

	for(env = environ; *env != NULL; env++){
		printf("environ=%s\n",*env);
	}

	return 0;
}
