#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc,char *argv[]){
	void *libHandle;
	void (*funcp)(void);
	const char *err;

	if(argc != 3 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"%s lib-path func-name\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	//libHandle = dlopen(argv[1],RTLD_LAZY);
	libHandle = dlopen(argv[1],RTLD_NOW);
	if(libHandle == NULL){
		fprintf(stderr,"dlopen:%s\n",dlerror());
		exit(EXIT_FAILURE);
	}

	printf("haha\n");
	dlerror();
	*(void **)(&funcp) = dlsym(libHandle,argv[2]);
	err = dlerror();
	if(err != NULL){
		fprintf(stderr,"dlsym:%s\n",err);
		exit(EXIT_FAILURE);
	}
	printf("hehe\n");

	if(funcp == NULL){
		printf("%s is NULL\n",argv[2]);
	}else{
		(*funcp)();
	}
	printf("xixi\n");

	dlclose(libHandle);

	return 0;
}
