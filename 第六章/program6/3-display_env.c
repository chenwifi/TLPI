#include <stdio.h>

extern char **environ;

int main(){
	char **env;

	for(env = environ;*env != NULL;env++){
		puts(*env);
	}

	return 0;
}
