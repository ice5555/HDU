#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>

//信号量名称定义
#define PIPE "/pipe"
#define SEND1 "/send1"
#define SEND2 "/send2"
#define SEND3 "/send3"
#define TEST "/test"
//定义传输信息
#define TXT1 "process1\n"
#define TXT2 "process2\n"
#define TXT3 "process3\n"
int main()
{
    int val=0;
    int fd[2];
    char buffer[50];        //设置字符缓冲区 

    sem_t *mutex, *send1, *send2, *send3, *test;   
    
    mutex = sem_open(PIPE, O_CREAT, 0644, 1); 
    send1 = sem_open(SEND1, O_CREAT, 0644, 0);
    send2 = sem_open(SEND2, O_CREAT, 0644, 0);
    send3 = sem_open(SEND3, O_CREAT, 0644, 0);
    test = sem_open(TEST, O_CREAT, 0644, 0);
    
    if (pipe(fd) == -1)
    {
        exit(-1);//创建无名管道,失败则跳出循环
    }
    //给三个子进程分配名称
    pid_t process1;
    pid_t process2;
    pid_t process3;
    pid_t process4; 
    
    
    if ((process1 = fork()) == 0)//fork用于创建一个子进程并返回一个pid,运行的是子进程的话
    {
        close(fd[0]);//关闭读端
        
        char p1[50] = TXT1;
        stpcpy(buffer, p1);
        sem_wait(mutex);//mutex-1,进程会被阻塞
        printf("阻塞1\n");       
        write(fd[1], buffer, 50);             
        sem_post(mutex);//使用 sem_post 函数释放信号量 mutex,mutex+1
        sem_post(send1);//使用 sem_post 函数发送信号 send1,send1+1
        printf("唤醒1\n");

        exit(0);
    }
    
    else if ((process2 = fork()) == 0)
    {
        close(fd[0]); 
        char p2[50] = TXT2;
        memset(buffer, 0, 50);
        stpcpy(buffer, p2);
        sem_wait(mutex);
        printf("阻塞2\n");
        write(fd[1], buffer, 50);
            
        sem_post(mutex);
        sem_post(send2);
        printf("唤醒2\n");
        exit(0);
    }
    
    else if ((process3 = fork()) == 0)
    {
        close(fd[0]);
        char p3[50] = TXT3;
        memset(buffer, 0, 50);
        stpcpy(buffer, p3);
        sem_wait(mutex);
        printf("阻塞3\n");
        write(fd[1], buffer, 50); 
        sem_post(mutex);
        sem_post(send3);
        printf("唤醒3\n");
        exit(0);
    }
    
    else
    {
    //子进程发送完数据以后在父进程中进行读取
    close(fd[1]); 
    memset(buffer, 0, 50);
    //等进程1、2、3都发送完所需要的数据才开始读取
    sem_wait(send1);
    sem_wait(send2);
    sem_wait(send3);
    sem_wait(mutex);
    //信号量值-1
    read(fd[0], buffer, 50);
    printf("第一次读取到的信息为：%s", buffer);
    sem_post(mutex);
    
    memset(buffer, 0, 50);
    sem_wait(mutex);
    read(fd[0], buffer, 50);
    printf("第二次读取到的信息为：%s", buffer);
    sem_post(mutex);
    memset(buffer, 0, 50);
    sem_wait(mutex);
    read(fd[0], buffer, 50);
    printf("第三次读取到的信息为：%s", buffer);
    sem_post(mutex);
    pipe(fd);
    sem_post(test); 
    }
    
    
    
    //测试管道默认大小
    if ((process4 = fork()) == 0){
        sem_wait(test);
        
        int i = 1;
        int size = 0;
        close(fd[0]);
        int state = fcntl(fd[1], F_SETFL,  O_NONBLOCK); 
        if (state < 0)
        {
            exit(1);
        }
        char *text = "textsize";
        while (++i)
        {
            sem_wait(mutex);
            val = write(fd[1], text, 8);
            sem_post(mutex);
            if (val < 0){
                break;//管道满了
            }
            size += 8;
        }
        printf("管道默认大小为%dKB\n",size/1024);
        exit(0);
    }else{
        close(fd[1]);//父进程的读端不能关闭，如果关闭了子进程写端会因为异常而退出
    }
    //释放信号量 
    sleep(3);
    sem_close(send1);
    sem_close(send2);
    sem_close(send3);
    sem_close(mutex);
    sem_close(test);
    //删除信号量 
    sem_unlink(PIPE);
    sem_unlink(SEND1);
    sem_unlink(SEND2);
    sem_unlink(SEND3);
    sem_unlink(TEST);
    printf("over!\n");
}