#include	<unpipc.h>

void
servproc(void *cookie, char *dataptr, size_t datasize,
		 door_desc_t *descptr, size_t ndesc)
{
	long	arg, result;
	
	arg = *((long *) dataptr);
	result = arg * arg;
	Door_return((char *) &result, sizeof(result), NULL, 0);
}

int
main(int argc, char **argv)
{
	int	fd;

	if (argc != 2)
		err_quit("usage: server2 <server-pathname>");

		/* 4create a door descriptor and attach to pathname */
	fd = Door_create(servproc, NULL, 0);

	unlink(argv[1]);
	Close(Open(argv[1], O_CREAT | O_RDWR, FILE_MODE));
	Fattach(fd, argv[1]);

		/* 4servproc() handles all client requests */
	for ( ; ; )
		pause();
}
