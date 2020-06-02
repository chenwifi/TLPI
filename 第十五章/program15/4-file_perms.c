#include <stdio.h>

#define STR_SIZE sizeof("rwxrwxrwx")
#define FP_SPECIAL 1

char *filePermStr(mode_t mode,int flag){
	char str[STR_SIZE];

	snprintf(str,STR_SIZE,"%c%c%c%c%c%c%c%c%c",
			(mode & S_IRUSR) ? 'r' : '-',(mode & S_IWUSR) ? 'w' : '-',
			(mode & S_IXUSR) ? (((mode & S_ISUID) && (flag & FP_SPECIAL)) ? 's' : 'x') : (((mode & S_ISUID) && (flag & FP_SPECIAL)) ? 'S' : '-') 
			);
}

int main(){

}
