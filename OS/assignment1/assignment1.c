#include <linux/module.h>

#include <unistd.h>
#include <linux/reboot.h>
 


int reboot(int cmd);

 
static int __init mytimer_init(void)
{
    reboot(SYS_RESTART);
	return 0;
}
 
static void __exit mytimer_exit(void)
{
	printk("exit!\n");
	
}
 
module_init(mytimer_init);
module_exit(mytimer_exit);
 
MODULE_LICENSE("GPL");




