/*
 * nfs-write.c
 *
 *  Created on: Dec 10, 2018
 *      Author: zhuweibo
 */


#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define     FILENAME_LENGTH     256




int main(int argc, char* argv[])
{
    const char lock_dir[] = "/proj/public/.tmp/em360/";
    char lock_file[FILENAME_LENGTH] = { 0 };

    snprintf(lock_file, FILENAME_LENGTH, "%s/build-nfs.loc", lock_dir);

    if (access(lock_dir, F_OK))
    {
        if (mkdir(lock_dir, 0755) == -1)
        {
            printf("mkdir %s failed\n", lock_dir);
            return EXIT_FAILURE;
        }
    }

    int fd = open(lock_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0)
    {
        printf("open %s failed\n", lock_file);
        return EXIT_FAILURE;
    }

    struct flock f;
    memset(&f, 0, sizeof(f));
    f.l_type = F_WRLCK;
    f.l_whence = SEEK_SET;
    f.l_start = 0;
    f.l_len = 0;

    if (fcntl(fd, F_SETLKW, &f) < 0)
    {
        printf("lock %s failed\n", lock_file);
        return EXIT_FAILURE;
    }

    char cmd[4096] = { 0 };
    int len = snprintf(cmd, sizeof(cmd), argv[1]);

    int i = 0;
    for (i = 2; i < argc; i++)
    {
        len += snprintf(cmd + len, sizeof(cmd) - len, " %s", argv[i]);
    }

    if (system(cmd))
    {
        printf("Execute cmd '%s' failed\n", cmd);
        return EXIT_FAILURE;
    }

    f.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &f) < 0)
    {
        printf("lock %s failed\n", lock_file);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
