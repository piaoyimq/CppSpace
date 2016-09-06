 

mqcreate1.c		指定排他性创建标志，创建一个消息队列

mqunlink.c		从系统中删除一个消息队列

mqgetattr.c		取得并输出某个消息队列的属性

mqcreate.c		mqcreate1.c的改进，允许指定属性

mqsend.c		往某个队列中增加一个消息

mqreceive.c		从某个队列中读入一个消息

mqsysconf.c		调用sysconf获取消息队列限制

mqnotifysig1.c		当有消息放置到某个空队列中时产生SIGUSR1（注意：它含有一个之后				要讨论的错误）

mqnotifysig2.c		信号处理程序只是给主线程设置一个标志（注意：它含有一个之后				要讨论的错误）

mqnotifysig3.c		使用信号通知读Posix消息队列

mqnotifysig4.c		伴随sigwait使用mq_notify

mqnotifysig5.c		伴随管道使用信号通知

mqnotifythread1.c	启动一个新线程

test1.c			演示实时信号的测试程序




