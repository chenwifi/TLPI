#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc,char *argv[]){
	int i;
	char **env;
	
	clearenv();

	for(i = 0;i < argc;i++){
		if(putenv(argv[i]) != 0){
			exit(EXIT_FAILURE);
		}
	}

	if(setenv("GREET","Hello world",0) == -1){
		exit(EXIT_FAILURE);
	}

	unsetenv("BYE");

	for(env = environ;*env != NULL;env++){
		puts(*env);
	}

	return 0;
}
