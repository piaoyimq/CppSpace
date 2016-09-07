/* include sem_trywait */
#include	<unpipc.h>
#include "../../10-posix-sem/10.16  my_pxsem_svsem  使用System V信号量实现Posix信号量/semaphore.h"

int
mysem_trywait(mysem_t *sem)
{
	struct sembuf	op;

	if (sem->sem_magic != SEM_MAGIC) {
		errno = EINVAL;
		return(-1);
	}

	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = IPC_NOWAIT;
	if (semop(sem->sem_semid, &op, 1) < 0)
		return(-1);
	return(0);
}
/* end sem_trywait */

int
Mysem_trywait(mysem_t *sem)
{
	int		rc;

	if ( (rc = mysem_trywait(sem)) == -1 && errno != EAGAIN)
		err_sys("mysem_trywait error");
	return(rc);
}
