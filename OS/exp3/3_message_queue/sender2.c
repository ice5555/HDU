#include "s_r.h"

int main()
{
    // 输入sender1创建的消息队列的id
    int msgid;
    printf("Input message id created by sender1:");
    scanf("%d", &msgid);
    printf("message id: %d\n", msgid);

    init_signal();

    char str[100];
    struct my_msgbuf s_msg;
    s_msg.mtype = 1;
    s_msg.sendid = 2; //表示由sender2进程发送

    while (1)
    {
        printf("\ntid:%u thread2 send to receiver: ", (unsigned int)pthread_self());
        scanf("%s", str);

        // 申请消息队列
        sem_wait(mutex);
        sem_wait(send);
        //  若用户输入exit退出
        if (strcmp(str, "exit") == 0)
        {
            // 发送end
            strcpy(s_msg.mtext, "end2");
            msgsnd(msgid, &s_msg, sizeof(struct my_msgbuf), 0);
            sem_post(receive);
            sem_post(mutex);
            break;
        }

        // 循环发送消息
        strcpy(s_msg.mtext, str);
        msgsnd(msgid, &s_msg, sizeof(struct my_msgbuf), 0);

        sem_post(receive);
        sem_post(mutex);
    }

    sem_wait(over2);
    // 接受到的消息
    struct my_msgbuf r_msg;
    msgrcv(msgid, &r_msg, sizeof(struct my_msgbuf), 0, 0);
    printf("Received from thread%d: %5s\n", r_msg.sendid, r_msg.mtext);

    sem_post(send);

    return 0;
}
