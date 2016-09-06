使用内存映射IO实现Posix消息队列：



mq_open.c		创建一个消息队列或打开一个已存在的消息队列

mq_close.c		撤销内存区映射，释放内存空间

mq_unlink.c		删除与我们定义的某个消息队列相关联的名字

mq_getattr.c		返回调用者指定的队列的当前属性

mq_setattr.c		给调用者指定的队列设置当前属性

mq_notify.c		注册或注销所指定队列的调用进程

mq_send.c		mq_send函数

mq_receive.c		mq_receive函数



