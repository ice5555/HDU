/*
 *  linux/kernel/reboot.c
 *
 *  Copyright (C) 2013  Linus Torvalds
 */

#define pr_fmt(fmt)	"reboot_countdown: " fmt

#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/reboot.h>
#include <linux/syscalls.h>
#include <linux/syscore_ops.h>
#include <linux/uaccess.h>

static void time_func(struct timer_list * data)
{
	printk("jiffies=%lu\n",jiffies);
    del_timer(&timer);
   	//emergency_restart();
}



SYSCALL_DEFINE1(reboot_countdown,unsigned int,time){
    
	struct timer_list timer;
	//module_param(time,int,0644);把这个更换成和用户态要数据

   	printk("hello world ???\n");
    //init_timer(&timer);
	timer.expires = jiffies + time*HZ;
	timer_setup(&timer,time_func,0);
    //timer.function=time_func;
    add_timer(&timer);
	return 0;

}

