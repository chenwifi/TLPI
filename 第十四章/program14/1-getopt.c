#include <stdio.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	int ch;
	
	printf("optind:%d---opterr:%d\n",optind,opterr);

	while((ch = getopt(argc,argv,"ab:c:de::")) != -1){
		printf("optind:%d\n",optind);
		switch (ch){
			case 'a':
				printf("Have option -a\n");
				break;
			case 'b':
				printf("Have option -b\n");
				printf("%s\n",optarg);
				break;
			case 'c':
				printf("Have option -c\n");
				printf("%s\n",optarg);
				break;
			case 'd':
				printf("Have option -d\n");
				printf("%s\n",optarg);
				break;
			case 'e':
				printf("Have option -e\n");
				printf("%s\n",optarg);
				break;
			case '?':
				printf("unknown option: %c\n\n",optopt);
				break;
		}
	}

	return 0;
}
