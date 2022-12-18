#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/reboot.h>

struct timer_list timer;
static int time =5;
module_param(time,int,0644);

static void time_func()
{
	
    printk("jiffies=%lu\n",jiffies);
    //printk(KERN_INFO"%s:IN init\n",__func__);
    
    //sync();
    //kernel_restart("restarting kernel");
    emergency_restart();
    
}

static int __init mytimer_init(void)
{
   	printk("hello world ???\n");
    timer_setup(&timer,time_func,0);
    printk("hello world ?\n");
	timer.expires = jiffies + time*HZ;
    add_timer(&timer);
    printk("hello world 5?\n");
    del_timer(&timer);
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




