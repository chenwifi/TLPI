#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/inotify.h>

static void displayInotifyEvent(struct inotify_event *);

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int main(int argc,char *argv[]){
	int inotifyfd,i,wd,len;
	char buf[BUF_LEN];
	ssize_t numRead;
	struct inotify_event *event;
	char *ino;

	inotifyfd = inotify_init();

	for(i = 1;i < argc;i++){
		wd = inotify_add_watch(inotifyfd,argv[i],IN_ALL_EVENTS);
		if(wd == -1){
			exit(EXIT_FAILURE);
		}
		printf("Watching %s using wd %d\n",argv[i],wd);
	}

	len = sizeof(struct inotify_event);
	while(1){
		numRead = read(inotifyfd,buf,BUF_LEN);
		/*
		ino = (struct inotify_event *)buf;

		for(;(ino + len + ino->len) < (buf + numRead); (ino = ino + len + ino->len)){
			displayInotifyEvent(ino);
		}
		*/

		for(ino = buf;ino < buf + numRead;){
			event = (struct inotify_event *)ino;
			displayInotifyEvent(event);
			ino += len + event->len;
		}
	}

	return 0;
}

static void displayInotifyEvent(struct inotify_event *event){
	printf("	wd = %2d; ",event->wd);
	if(event->cookie > 0){
		printf("cookie = %4d; ",event->cookie);
	}

	printf("mask = ");
	if(event->mask & IN_CREATE) printf(" IN_CREATE ");
	if(event->mask & IN_OPEN) printf(" IN_OPENE ");
	if(event->mask & IN_DELETE_SELF) printf(" IN_DELETE_SELF ");
	if(event->mask & IN_IGNORED) printf(" IN_IGNORED ");
	if(event->mask & IN_ISDIR) printf(" IN_ISDIR ");
	if(event->mask & IN_MODIFY) printf(" IN_MODIFYE ");
	if(event->mask & IN_MOVED_FROM) printf(" IN_MOVED_FROME ");
	if(event->mask & IN_MOVED_TO) printf(" IN_MOVED_TO ");
	if(event->mask & IN_CLOSE_WRITE) printf(" IN_CLOSE_WRITE ");

	if(event->len > 0){
		printf("	name=%s\n",event->name);
	}
}
