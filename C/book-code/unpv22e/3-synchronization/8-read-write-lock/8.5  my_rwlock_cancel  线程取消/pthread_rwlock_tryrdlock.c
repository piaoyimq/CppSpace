/* include tryrdlock */
#include	<unpipc.h>
#include "../../8-read-write-lock/8.5  my_rwlock_cancel  线程取消/pthread_rwlock.h"

int
pthread_rwlock_tryrdlock(pthread_rwlock_t *rw)
{
	int		result;

	if (rw->rw_magic != RW_MAGIC)
		return(EINVAL);

	if ( (result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return(result);

	if (rw->rw_refcount == -1 || rw->rw_nwaitwriters > 0)
		result = EBUSY;			/* held by a writer or waiting writers */
	else
		rw->rw_refcount++;		/* increment count of reader locks */

	pthread_mutex_unlock(&rw->rw_mutex);
	return(result);
}
/* end tryrdlock */

int
Pthread_rwlock_tryrdlock(pthread_rwlock_t *rw)
{
	int		n;

	if ( (n = pthread_rwlock_tryrdlock(rw)) != 0) {
		errno = n;
		err_sys("pthread_rwlock_tryrdlock error");
	}
	return(n);
}
