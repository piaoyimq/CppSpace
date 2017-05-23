loopmain.c		文件上锁
lockfcntl.c		Posix fcntl上锁

test2.c			确定在有一个写入锁待处理期间是否允许有另一个读出锁

test3.c			测试写入者是否比读出者优先

onedaemon.c		确保某个程序只有一个副本在运行

lockopen.c		使用指定O_CREAT和O_EXCL标志的open实现的锁函数

lockpxsem.c		使用Posix信号量的文件上锁

locksvsem.c		使用System V信号量实现的文件上锁