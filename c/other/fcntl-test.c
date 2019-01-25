#include <Akita/Akita.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, const char *argv[])
{
    int fd = open("./main.c", O_RDWR);
    if (fd < 0)
    {
        printf("%s\n", "Open File error!");
    }
    int ld = flock(fd, LOCK_EX);
    struct flock lock;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_type = F_WRLCK;
    int fld = fcntl(fd, F_SETLK, &lock);
    printf("FCNTL ret = %d\n", fld);
    if (fld == 0)
    {
        if (lock.l_type == F_WRLCK)
        {
            printf("WRLCK PID = %d\n", getpid());
        }
    }
    getchar();
    return 0;
}
}
