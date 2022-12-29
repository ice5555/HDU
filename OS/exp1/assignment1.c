#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/reboot.h>


static int time =2;
module_param(time,int,0644);
struct timer_list timer;
static void time_func(struct timer_list * data)
{
 
    del_timer(&timer);
    emergency_restart();
    
}

static int __init mytimer_init(void)
{
    
   	printk("即将在%d秒后重启\n",time);

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
 
MODULE_LICENSE("GPL");




