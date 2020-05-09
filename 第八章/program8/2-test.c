#define _XOPEN_SOURCE
#include <stdio.h>
#include <shadow.h>
#include <unistd.h>

int main(){
	char *buf;
	struct spwd *pwd;
	int a;

	buf = crypt("123456","ab");
	printf("%s\n",buf);
	pwd = getspnam("bigfat");
	printf("%s\n",pwd->sp_pwdp);
	buf = crypt("123456",pwd->sp_pwdp);
	printf("%s\n",buf);

	a = (strcmp(buf,pwd->sp_pwdp) == 0);
	printf("%d\n",a);

	return 0;
}
