#include <stdio.h>
#include <errno.h>
#include <unistd.h>

ssize_t readn(int fd,void *buf,size_t n){
	size_t totalRead;
	ssize_t numRead;
	char *buffer;

	buffer = buf;
	for(totalRead = 0; totalRead < n; ){
		numRead = read(fd,buffer,n - totalRead);

		if(numRead == 0){
			return totalRead;
		}

		if(numRead < 0){
			if(numRead == -1 && errno == EINTR){
				continue;
			}else{
				return -1;
			}
		}

		totalRead += numRead;
		buf += numRead;
	}

	return totalRead;
}

ssize_t writen(int fd,void *buf,size_t n){
	size_t totalWrite;
	ssize_t numWrite;
	char *buffer;

	buffer = buf;
	for(totalWrite = 0; totalWrite < n){
		numWrite = write(fd,buffer,n - totalWrite);
		if(numWrite < 0){
			if(numWrite == -1 && errno == EINTR){
				continue;
			}else{
				return -1;
			}
		}

		totalWrite += numWrite;
		buffer += numWrite;
	}

	return totalWrite;
}
