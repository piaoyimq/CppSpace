#include "../../6-system-V-message-queue/6.7  svmsgcliserv  客户服务器例子/svmsg.h"

void	server(int, int);

int
main(int argc, char **argv)
{
	int		readid, writeid;

	readid = Msgget(MQ_KEY1, SVMSG_MODE | IPC_CREAT);
	writeid = Msgget(MQ_KEY2, SVMSG_MODE | IPC_CREAT);

	server(readid, writeid);

	exit(0);
}
