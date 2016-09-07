#include	<unpipc.h>
#include "../../5-posix-message-queue/5.8  my_pxmsg_mmap  使用内存映射IO实现Posix消息队列/mqueue.h"

int
main(int argc, char **argv)
{
	if (argc != 2)
		err_quit("usage: mqunlink <name>");

	Mymq_unlink(argv[1]);

	exit(0);
}
