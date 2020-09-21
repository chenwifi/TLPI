#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define handle_error_en(en,msg) \
	do {errno = en; perror(msg); exit(EXIT_FAILURE);} while(0)

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE);} while(0)

struct thread_info{
	pthread_t thread_id;
	int thread_num;
	char *argv_string;
};

static void *thread_start(void *argv){
	struct thread_info *tinfo;
	char *uargv,*p;

	tinfo = (struct thread_info *)argv;

	printf("Thread %d: top of stack near %p; argv_string = %s\n",tinfo->thread_num,&p,tinfo->argv_string);

	uargv = strdup(tinfo->argv_string);
	if(uargv == NULL){
		handle_error("strdup");
	}

	for(p = uargv; *p != '\0'; p++){
		*p = toupper(*p);
	}

	return uargv;
}

int main(int argc,char *argv[]){
	int stack_size;
	int s,opt,num_threads,tnum;
	pthread_attr_t attr;
	struct thread_info *tinfo;
	void *res;

	stack_size = -1;
	while((opt = getopt(argc,argv,"s:")) != -1){
		switch(opt){
			case 's':
				stack_size = strtoul(optarg,NULL,0);
				break;
			default:
				fprintf(stderr,"Usage: %s [-s stacksize]\n",argv[0]);
				exit(EXIT_FAILURE);
				break;
		}
	}

	num_threads = argc - optind;

	s = pthread_attr_init(&attr);
	if(s != 0){
		handle_error_en(s,"pthread_attr_init");
	}

	if(stack_size > 0){
		s = pthread_attr_setstacksize(&attr,stack_size);
		if(s != 0){
			handle_error_en(s,"pthread_attr_setstacksize");
		}
	}

	tinfo = (struct thread_info *)calloc(num_threads,sizeof(struct thread_info));
	if(tinfo == NULL){
		handle_error("calloc");
	}

	for(tnum = 0; tnum < num_threads; tnum++){
		tinfo[tnum].thread_num = tnum + 1;
		tinfo[tnum].argv_string = argv[optind + tnum];

		s = pthread_create(&tinfo[tnum].thread_id,&attr,thread_start,(void *)&tinfo[tnum]);
		if(s != 0){
			handle_error_en(s,"pthread_create");
		}
	}

	s = pthread_attr_destroy(&attr);
	if(s != 0){
		handle_error_en(s,"pthread_attr_destroy");
	}

	for (tnum = 0; tnum < num_threads; tnum++) {
		s = pthread_join(tinfo[tnum].thread_id,&res);
		if(s != 0){
			handle_error_en(s,"pthread_join");
		}

		printf("join with thread %d; return value was %s\n",tinfo[tnum].thread_num,(char *)res);

		free(res);
	}

	free(tinfo);

	return 0;
}
