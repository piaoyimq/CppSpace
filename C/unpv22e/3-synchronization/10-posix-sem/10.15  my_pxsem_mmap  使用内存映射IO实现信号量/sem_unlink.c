/* include sem_unlink */
#include	<unpipc.h>
#include "../../10-posix-sem/10.15  my_pxsem_mmap  使用内存映射IO实现信号量/semaphore.h"

int
mysem_unlink(const char *pathname)
{
	if (unlink(pathname) == -1)
		return(-1);
	return(0);
}
/* end sem_unlink */

void
Mysem_unlink(const char *pathname)
{
	if (mysem_unlink(pathname) == -1)
		err_sys("mysem_unlink error");
}

