/* include sem_getvalue */
#include	<unpipc.h>
#include "../../10-posix-sem/10.16  my_pxsem_svsem  使用System V信号量实现Posix信号量/semaphore.h"

int
mysem_getvalue(mysem_t *sem, int *pvalue)
{
	int		val;

	if (sem->sem_magic != SEM_MAGIC) {
		errno = EINVAL;
		return(-1);
	}

	if ( (val = semctl(sem->sem_semid, 0, GETVAL)) < 0)
		return(-1);
	*pvalue = val;
	return(0);
}
/* end sem_getvalue */

void
Mysem_getvalue(mysem_t *sem, int *pvalue)
{
	if (mysem_getvalue(sem, pvalue) == -1)
		err_sys("mysem_getvalue error");
}

