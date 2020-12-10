#include <sys/types.h>

#define SERVER_FIFO "/root/TLPI/program44/fifoprog/seqnum_sv"
#define CLIENT_FIFO_TEMPLATE "/root/TLPI/program44/fifoprog/seqnum_cl.%ld"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

struct request{
	pid_t pid;
	int seqLen;
};

struct response{
	int seqNum;
};
