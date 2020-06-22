#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

int main(int argc,char *argv[]){
	char *ch1,*ch2;
	int i;

	for(i = 1;i < argc; i++){
		ch1 = strdup(argv[i]);
		ch2 = strdup(argv[i]);

		printf("%s -> %s : %s\n",argv[i],dirname(ch1),basename(ch2));

		free(ch1);
		free(ch2);
	}

	return 0;
}
