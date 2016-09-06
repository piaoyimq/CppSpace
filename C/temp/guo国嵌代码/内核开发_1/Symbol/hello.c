#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Xie");
MODULE_DESCRIPTION("Hello World Module");
MODULE_ALIAS("a simplest module");

extern int add_integar(int a,int b); 
extern int sub_integar(int a,int b);

static int __init hello_init()
{
    int res = add_integar(1,2);
    return 0;
}

static void __exit hello_exit()
{
    int res = sub_integar(2,1);
}

module_init(hello_init);
module_exit(hello_exit);
