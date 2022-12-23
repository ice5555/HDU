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
	
    printk("jiffies546=%lu\n",jiffies);
    //printk(KERN_INFO"%s:IN init\n",__func__);
    del_timer(&timer);
    //sync();
    //kernel_restart("restarting kernel");
    emergency_restart();
    
}

static int __init mytimer_init(void)
{
    
   	printk("hello world ???\n");
    //printk("jiffies=%lu\n",jiffies);
	timer.expires = jiffies + time*HZ;
    //printk("jiffies=%lu\n",jiffies);
    timer_setup(&timer,time_func,0);
    //printk("jiffies=%lu\n",jiffies);
    add_timer(&timer);
    
	return 0;
}
 
static void __exit mytimer_exit(void)
{
	printk("exit!\n");
	printk(KERN_INFO"%s:IN init\n",__func__);
}
 
module_init(mytimer_init);
module_exit(mytimer_exit);
 
MODULE_LICENSE("GPL");




