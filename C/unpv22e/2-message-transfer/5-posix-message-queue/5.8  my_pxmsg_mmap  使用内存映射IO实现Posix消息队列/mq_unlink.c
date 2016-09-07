/* include mq_unlink */
#include	<unpipc.h>
#include "../../5-posix-message-queue/5.8  my_pxmsg_mmap  使用内存映射IO实现Posix消息队列/mqueue.h"

int
mymq_unlink(const char *pathname)
{
	if (unlink(pathname) == -1)
		return(-1);
	return(0);
}
/* end mq_unlink */

void
Mymq_unlink(const char *pathname)
{
	if (mymq_unlink(pathname) == -1)
		err_sys("mymq_unlink error");
}
