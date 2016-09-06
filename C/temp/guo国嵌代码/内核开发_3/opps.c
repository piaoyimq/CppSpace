#define __KERNEL__
#define MODULE

#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h>

int D()
{
    int *p = NULL;
    int a = 6;
    printk("Function D\n");
    *p = a+5;	
}

int C()
{
    printk("Function C\n");
    D();	
}

int B()
{
    printk("Function B\n");
    C();	
}

int A()
{
    printk("Function A\n");
    B();	
}

int opps_init()
{
    printk("hi\n");
    A();
    return 0;
}

void opps_exit()
{

}

module_init(opps_init);
module_exit(opps_exit);