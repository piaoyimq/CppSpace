#include <linux/init.h>                                
#include <linux/module.h>                                

MODULE_LICENSE("GPL"); 
         
static char *name = "David Xie";              
static int age = 30;    

module_param(age, int, S_IRUGO);                                
module_param(name, charp, S_IRUGO);                             
                                
static int hello_init(void)                                
{                                
	printk(KERN_EMERG" Name:%s\n",name);                        
	printk(KERN_INFO" Age:%d\n",age);                               
	return 0;                                
}                                
static void hello_exit(void)                                
{                                
	printk(KERN_INFO" Module Exit\n ");                            
}
                                
module_init(hello_init);                                
module_exit(hello_exit);                                                                
