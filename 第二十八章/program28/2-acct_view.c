#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/acct.h>
#include <pwd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define TIME_BUF_SIZE 100

char * userNameFromId(uid_t uid){
	struct passwd *pwd;

	pwd = getpwuid(uid);
	return (pwd==NULL) ? NULL : pwd->pw_name;
}

static long long comptToLL(comp_t ct){
	const int EXP_SIZE = 3;
	const int MANTISSA_SIZE = 13;
	const int MANTISSA_MASK = (1 << MANTISSA_SIZE) - 1;
	long long mantissa,exp;

	mantissa = ct & MANTISSA_MASK;
	exp = (ct >> MANTISSA_SIZE) & ((1 << EXP_SIZE) - 1);

	return mantissa << (exp * 3);
}

int main(int argc,char *argv[]){
	int acctFile;
	struct acct ac;
	ssize_t numRead;
	char *s;
	struct tm *loc;
	char timeBuf[TIME_BUF_SIZE];
	time_t t;

	if(argc != 2 || strcmp(argv[1],"--help") == 0){
		fprintf(stderr,"usage: %s file\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	acctFile = open(argv[1],O_RDONLY);
	if(acctFile == -1){
		fprintf(stderr,"open error\n");
		exit(EXIT_FAILURE);
	}

	printf("command  flags   term.status  user		start time			CPU		elapsed\n");

	while((numRead = read(acctFile,&ac,sizeof(struct acct))) > 0){
		if(numRead != sizeof(struct acct)){
			fprintf(stderr,"partial read\n");
			exit(EXIT_FAILURE);
		}

		printf("%-8.8s ",ac.ac_comm);

		printf("%c",(ac.ac_flag & AFORK) ? 'F' : '-');
		printf("%c",(ac.ac_flag & ASU) ? 'S' : '-');
		printf("%c",(ac.ac_flag & AXSIG) ? 'X' : '-');
		printf("%c",(ac.ac_flag & ACORE) ? 'C' : '-');

		printf("%#6lx	",(unsigned long) ac.ac_exitcode);

		s = userNameFromId(ac.ac_uid);
		printf("%-8.8s ",(s == NULL) ? "???" : s);

		t = ac.ac_btime;
		loc = localtime(&t);
		if(loc == NULL){
			printf("???unknown time???	");
		}else{
			strftime(timeBuf,TIME_BUF_SIZE,"%Y-%m-%d %T ",loc);
			printf("%s ",timeBuf);
		}

		printf("%5.2f %7.2f ",(double)(comptToLL(ac.ac_utime) + comptToLL(ac.ac_stime)) / sysconf(_SC_CLK_TCK),(double) comptToLL(ac.ac_etime) / sysconf(_SC_CLK_TCK));
		printf("\n");
	}

	if(numRead == -1){
		fprintf(stderr,"read error\n");
	}

	return 0;
}
