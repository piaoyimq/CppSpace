#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Xie");
MODULE_DESCRIPTION("Memory alloc Module");
MODULE_ALIAS("malloc module");

char *buf1 = NULL;
char *buf2 = NULL;


int alloc_init()
{
	buf1 = kmalloc(100,GFP_KERNEL);
	memset(buf1,0,100);
	strcpy(buf1,"<<< --- Kmalloc Mem OK! --- >>>");
	printk("<0>BUF 1 : %s\n",buf1);
	
	buf2 = get_zeroed_page(GFP_KERNEL);
	strcpy(buf2,"<<<--- Get Free Page OK! --- >>>");
	printk("<0>BUF 2 : %s\n",buf2);
	return 0;
}


void alloc_exit()
{
	kfree(buf1);
	free_page(buf2);
	printk("<0><<< --- Module Exit! --->>>\n");
}

module_init(alloc_init);
module_exit(alloc_exit);
