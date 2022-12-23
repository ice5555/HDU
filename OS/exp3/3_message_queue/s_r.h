#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
// 消息缓冲队列通信机制是消息传递系统通信中直接通信方式的一种具体实现，基本思想为：
// 通常由系统统一管理一组用于通信的空闲消息缓冲区；某进程要发送消息时，首先在自己的地址空间中设置一个发送区，
// 并把欲发送的消息填入其中形成消息，再申请一个消息缓冲区，把数据从发送区复制到消息缓冲区中，
// 然后再把该消息缓冲区直接发送到接收进程的消息队列里；
// 接收进程从自己的消息队列上取下消息缓冲区，并将其中的数据复制到自己的消息接收区中，最后释放消息缓冲区。

/* 
extern int sem_init __P ((sem_t *__sem, int __pshared, unsigned int __value));
头文件为： #include <semaphore.h>
sem为指向信号量结构的一个指针；
pshared不为0时此信号量在进程间共享，否则只能为当前进程的所有线程共享；
value给出了信号量的初始值。
*/

//定义后续所需要的有名信号量以及关键key
// 五个有名Posix信号量：
// 对消息队列的互斥信号量mutex，同步信号量send、receive、over1、over2
#define KEY 100
sem_t *mutex; //sem_t 本质是长整型数
sem_t *send;    //定义长整型指针
sem_t *receive;
sem_t *over1;
sem_t *over2;

//初始化msg.h里的msgbuf——my_msgbuf——IPC消息队列中的消息缓冲区
struct my_msgbuf
{
    long mtype;      //1-send, 2-recv  对应消息类型 必须是一个大于0的长整数
    int sendid;      //自己定义的参数  用于确定进程通信
    char mtext[100]; //对应正文长度  可以为0 最大为MSGMAX即8192
};

//封装信号量初始化 init_singal 和删除 clean_singal
/*
sem_t *sem_open
(const char *name, int oflag, mode_t mode, unsigned int value);
打开一个已存在的有名信号量或创建初始化一个有名信号量
参数：
name:有名信号量的名称  会创建在 /dev/shm/shm.name 下
oflag:创建方式  有O_CREAT或O_CREAT|O_EXCL两种取值
            O_CREAT：若name参数指定的信号量存在，直接打开该信号量，
                    忽略mode和value参数 否则创建信号量。
            O_CREAT|O_EXCL：若name参数指定的信号量存在，直接抛出异常，
                    否则创建信号量。
mode:权限位  如0644代表rw-r--r--
value:信号量初始值  最大值为SEM_VALUE_MAX即32767，二值信号量的初始值通常为1，计数信号量的初始值则往往大于1。
返回值：成功时返回指向有名信号量的指针，该结构里记录着当前共享资源的数目，出错时为SEM_FAILED(常量值=2)
*/
void init_signal()
{
    mutex = sem_open("mutex", O_CREAT, 0666, 1);
    send = sem_open("send", O_CREAT, 0666, 1);
    receive = sem_open("receive", O_CREAT, 0666, 0);
    over1 = sem_open("over1", O_CREAT, 0666, 0);
    over2 = sem_open("over2", O_CREAT, 0666, 0);
}

// 位于#include <semaphore.h>
/*
int sem_close(sem_t *sem);
    功能：将信号量引用计数减1，并不会删除该信号量，之后可以通过sem_open()打开
    参数：sem:指向欲关闭的信号量的指针，即调用sem_open()的返回值；
    返回值：成功返回0 失败返回-1
*/
/*
int sem_unlink(const char *name);
    功能：从系统中彻底删除该信号量，只对引用计数为0的信号量有用
    参数：name:有名信号量的名称  会创建在 /dev/shm/shm.name 下
    返回值：成功返回0 失败返回-1
*/
void clean_signal()
{
    sem_unlink("mutex");
    sem_unlink("send");
    sem_unlink("receive");
    sem_unlink("over1");
    sem_unlink("over2");
}
