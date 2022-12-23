#include "s_r.h"
#include <unistd.h>
int main()
{
    // 获取消息队列id
    int msgid;
    printf("Input message id created by sender1:");
    scanf("%d", &msgid);
    printf("message id: %d\n", msgid);

    init_signal();

    struct my_msgbuf r_msg; //接受
    struct my_msgbuf s_msg; //发送
    s_msg.mtype = 2;        // 1对应发送，2对应接收
    s_msg.sendid = 3;       // 确定通信的进程
    int flag_over1 = 0;
    int flag_over2 = 0;

    while (1)
    {
        sem_wait(receive); // 申请

        msgrcv(msgid, &r_msg, sizeof(struct my_msgbuf), 0, 0);
        printf("Received from thread%d: %5s\n", r_msg.sendid, r_msg.mtext);

        if (r_msg.sendid == 1) //sender1
        {
            if (strcmp(r_msg.mtext, "end1") == 0)
            {
                printf("send over1.\n");
                strcpy(s_msg.mtext, "over1");
                msgsnd(msgid, &s_msg, sizeof(struct my_msgbuf), 0);

                sem_post(over1);
                flag_over1 = 1;
            }
            else
            {
                sem_post(send);
            }
        }
        else if (r_msg.sendid == 2) //sender2
        {
            if (strcmp(r_msg.mtext, "end2") == 0)
            {
                printf("send over2.\n");
                strcpy(s_msg.mtext, "over2");
                msgsnd(msgid, &s_msg, sizeof(struct my_msgbuf), 0);

                sem_post(over2);
                flag_over2 = 1;
            }
            else
            {
                sem_post(send);
            }
        }

        if (flag_over1 && flag_over2) //两个进程都结束
            break;
    }

    // 关闭信号量 s_r.h中
    clean_signal();
    sleep(3);
    msgctl(msgid, IPC_RMID, 0); //删除消息队列
    /*  int msgctl(int msqid, int cmd, struct msqid_ds *buf)
        功能:获取或设置消息队列的属性信息，或者删除消息队列。
        参数:  msqid:消息队列的标识符
        cmd:
        IPC_STAT：取此队列的msqid_ds结构体，并将它存放在buf指向的结构中
        IPC_SET：将字段msg_perm.uid、msg_perm.gid、msg_perm.mode、msg_qbytes从buf所指向的结构复制到与这个队列相关的msqid_ds结构中。
                    此命令只能由下列两种进程执行：一种是其有效用户ID等于msg_perm.uid或msg_perm.cuid。
                    另一种是具有超级用户特权的进程。但只有超级用户才能增加msg_qbytes的值
        IPC_RMID：从系统中删除msqid参数指定的消息队列以及仍在该队列中的所有数据，这种删除立即生效。
                    仍在使用这一消息队列的其它进程在它们下一次试图对此队列进行操作时，将得到EIDRM错误
                    此命令只能由下列两种进程执行：一种是其有效用户ID等于msg_perm.uid或msg_perm.cuid。另一种是具有超级用户特权的进程
        IPC_INFO（特定于Linux）：返回buf指向的结构中有关系统范围消息队列限制和参数的信息。
                    此结构属于msginfo类型（因此，需要强制转换）。
                    如果定义了gnu_source feature test宏，则在<sys/msg.h>中定义（可以通过同名的/proc文件更改msgmni、msgmax和msgmnb设置；
                    有关详细信息，请参阅proc（5））
        buf：消息队列管理结构体，用户空间中的一个缓存，接受或提供状态信息
  返回值:成功执行返回0,否则返回-1.
    */

    // struct msqid_ds=msg_queue {
    //     struct ipc_perm msg_perm;     /* Ownership and permissions */
    //     time_t          msg_stime;    /* Time of last msgsnd(2) */
    //     time_t          msg_rtime;    /* Time of last msgrcv(2) */
    //     time_t          msg_ctime;    /* Time of last change */
    //     unsigned long   __msg_cbytes; /* Current number of bytes inqueue (nonstandard) */
    //     msgqnum_t       msg_qnum;     /* Current number of messages in queue */
    //     msglen_t        msg_qbytes;   /* Maximum number of bytes allowed in queue */
    //     pid_t           msg_lspid;    /* PID of last msgsnd(2) */
    //     pid_t           msg_lrpid;    /* PID of last msgrcv(2) */
    // };

    return 0;
}
