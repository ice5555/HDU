#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/reboot.h>
 
struct timer_list timer;
static int time =5;
module_param(time,int,0644);

static void time_func(struct timer_list * data)
{
	
    printk("jiffies=%lu\n",jiffies);
    del_timer(&timer);
    //sync()
    //reboot(LINUX_REBOOT_CMD_RESTART);
    
}
 
static int __init mytimer_init(void)
{
    printk("hello world ???\n");
  
	timer.expires = jiffies + time*HZ;
    timer_setup(&timer,time_func,0);

    add_timer(&timer);
	return 0;
}
 
static void __exit mytimer_exit(void)
{
	printk("exit!\n");
	
}
 
module_init(mytimer_init);
module_exit(mytimer_exit);
 
MODULE_LICENSE("Dual BSD/GPL");