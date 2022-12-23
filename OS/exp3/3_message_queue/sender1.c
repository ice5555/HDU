#include "s_r.h"

int main()
{
    int msgid;
    /* 创建并打开消息队列：
    int msgget(key_t key, int msgflag)
    功能:用于创建一个新的或打开一个已经存在的消息队列，此消息队列与key相对应。
    参数:
        key:消息队列的键值，为0(IPC_PRIVATE)则创建一个新的消息队列；>0通过ftok函数创建 
        msgflag:对消息队列访问权限和控制命令的组合
            IPC_CREAT:创建新的消息队列。 
            IPC_EXCL:与IPC_CREAT一同使用，表示如果要创建的消息队列已经存在，则返回错误。 
            IPC_NOWAIT:读写消息队列要求无法满足时，不阻塞。
    返回值:调用成功返回队列标识符,否则返回-1.
*/

    msgid = msgget(KEY, 0666 | IPC_CREAT);
    printf("Allocated message id: %d\n", msgid);

    init_signal(); //初始化信号量

    char str[100];
    struct my_msgbuf s_msg; //s_r.h中：msg.h里的msgbuf——my_msgbuf——IPC消息队列中的消息缓冲区
    s_msg.mtype = 1;
    s_msg.sendid = 1;

    while (1)
    {
        memset(str, '\0', strlen(str));
        /*  pthread_self(void)  pthread_t
                函数作用：获得线程自身的ID。pthread_t的类型为unsigned long int，
                所以在打印的时候要使用%lu方式，否则显示结果出问题。 */
        printf("\ntid:%u thread1 send to receiver: ", (unsigned int)pthread_self());
        scanf("%s", str); //输入要发送的信息到数组中

        sem_wait(mutex);
        sem_wait(send);

        if (strcmp(str, "exit") == 0) //输入exit，表示不再发送
        {
            //可循环发送多个消息，直到用户输入“exit”为止，
            // 表示它不再发送消息，最后向receiver线程发送消息“end1"
            strcpy(s_msg.mtext, "end1");
            msgsnd(msgid, &s_msg, sizeof(struct my_msgbuf), 0);

            /*   int sem_post(sem_t *sem);  唤醒等待进程
            功能:  将指定的信号量值+1 若有线程/进程在等待，则会唤醒其中一个线程/进程
            参数: sem 指定的信号量名称
            返回值:调用成功返回0,否则返回-1,信号量的值不做改动 */
            sem_post(receive);
            sem_post(mutex);
            break;
        }

        strcpy(s_msg.mtext, str); //发送的不是exit，则循环发送信息
                                  /* int msgsnd(int msqid, struct msgbuf *msgp, size_t msgsz, int msgflag)
        功能:向标识符msqid的消息队列中发送一个信息。
        参数:msqid:消息队列的标识符
            msgp:存放欲发送消息内容的消息缓冲区指针
            msgsz:消息正文部分的长度
            msgflag:发送标志
            0:消息队列满时，调用发送进程会被阻塞，直到消息队列可写入信息
            IPC_NOWAIT:消息队列满时，调用进程立即返回-1
            MSG_NOERROR:消息正文长度超过msgsz时，不报错直接截取多余部分
        返回值:消息发送成功返回0,否则返回-1. */
        msgsnd(msgid, &s_msg, sizeof(struct my_msgbuf), 0);

        sem_post(receive);
        sem_post(mutex);
    }

    sem_wait(over1);

    struct my_msgbuf r_msg;
    /*  size_t  msgrcv(int msqid, struct msgbuf *msgp, size_t msgsz, long msgtyp, int msgflag)
    功能:若msgflg是IPC_NOWAIT，并没有可接受的消息时返回ENOMSG=80，如果消息队列被删除返回EIDRM=81，捕获到信号返回EINTR=4。
    参数:  msqid:消息队列的标识符
            msgp:存放欲发送消息内容的消息缓冲区指针
            msgsz:消息正文部分的长度
            msgtyp:接受的消息类型
            0:接受消息队列中的第一个信息
            >0:接受第一个类型为msgtyp的消息
            ><0:接受第一个类型小于等于msgtyp的绝对值的消息
            msgflag:发送标志
            0:没有可接受的消息时，调用接受进程会被阻塞
            IPC_NOWAIT:没有可接受的消息时，调用进程立即返回-1
            MSG_EXCEPT:返回第一个类型不是msgtyp的消息
            MSG_NOERROR:消息正文长度超过msgsz时，不报错直接截取多余部分
    返回值:消息接受成功返回消息正文的字节数,否则返回-1 */
    msgrcv(msgid, &r_msg, sizeof(struct my_msgbuf), 0, 0);
    // 缓冲区接收到的信息
    printf("Received from thread%d: %5s\n", r_msg.sendid, r_msg.mtext);

    sem_post(send);

    return 0;
}
