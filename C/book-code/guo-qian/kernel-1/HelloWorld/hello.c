#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Xie");
MODULE_DESCRIPTION("Hello World Module");
MODULE_ALIAS("a simplest module");

static int __init hello_init()
{
    printk(KERN_EMERG"Hello World!\n");
    return 0;
}

static void __exit hello_exit()
{
    printk("<6>hello exit\n");
}

module_init(hello_init);
module_exit(hello_exit);
