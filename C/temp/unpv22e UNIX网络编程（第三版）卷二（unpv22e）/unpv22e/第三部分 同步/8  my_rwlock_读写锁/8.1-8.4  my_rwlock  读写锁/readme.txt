
使用互斥锁和条件变量实现读写锁：

pthread_rwlock_init.c		初始化一个读写锁
pthread_rwlock_destroy.c	摧毁一个读写锁



pthread_rwlock_rdlock.c		获取一个读出锁
pthread_rwlock_tryrdlock.c	试图获取一个读出锁

pthread_rwlock_wrlock.c		获取一个写入锁
pthread_rwlock_trywrlock.c	试图获取一个写入锁


pthread_rwlock_unlock.c		释放一个读出锁或写入锁

testcancel.c			线程取消