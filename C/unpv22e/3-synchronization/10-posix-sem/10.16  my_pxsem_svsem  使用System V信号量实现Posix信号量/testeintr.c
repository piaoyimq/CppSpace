#include	<unpipc.h>
#include "../../10-posix-sem/10.16  my_pxsem_svsem  使用System V信号量实现Posix信号量/semaphore.h"

#define	NAME	"testeintr"

static void	sig_alrm(int);

int
main(int argc, char **argv)
{
	mysem_t	*sem1;

		/* 4test a named semaphore */
	mysem_unlink(Px_ipc_name(NAME));
	sem1 = Mysem_open(Px_ipc_name(NAME), O_RDWR | O_CREAT | O_EXCL,
					  FILE_MODE, 0);

	Signal(SIGALRM, sig_alrm);
	alarm(2);
	if (mysem_wait(sem1) == 0)
		printf("mysem_wait returned 0?\n");
	else
		err_ret("mysem_wait error");
	Mysem_close(sem1);

	exit(0);
}

static void
sig_alrm(int signo)
{
	printf("SIGALRM caught\n");
	return;
}
