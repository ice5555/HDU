#include <linux/module.h>
#include <linux/reboot.h>
//#include <unistd.h>

 /*
 *  linux/kernel/reboot.c
 *
 *  Copyright (C) 2013  Linus Torvalds
 */


//int reboot(int cmd);

 
static int __init mytimer_init(void)
{
    //reboot(SYS_RESTART);
	kernel_restart(NULL);
	return 0;
}
 
static void __exit mytimer_exit(void)
{
	printk("exit!\n");
	
}
 
module_init(mytimer_init);
module_exit(mytimer_exit);
 
MODULE_LICENSE("GPL");




