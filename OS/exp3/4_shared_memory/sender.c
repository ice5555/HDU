#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

sem_t *sem_empty;
sem_t *sem_full;
sem_t *sem_mutex;

int shmid;
void *shmp;

void my_init(){
    sem_empty = sem_open("empty", O_CREAT, 0666, 1);//创建并初始化信号量。name为empty，oflag为O_CREAT（创建一个信号量），
	                                                //权限为0666可读可写，value初始值为1 
    sem_full = sem_open("full", O_CREAT, 0666, 0);
    sem_mutex = sem_open("mutex", O_CREAT, 0666, 1);
    //分配共享内存 
    shmid = shmget(0x0127, 1024, 0666|IPC_CREAT);//key关键字，共享内存标识符，size申请1k字节大小，flag为权限标识 
    // 用来启动对该共享内存的访问，并把共享内存连接到当前进程的地址空间
    shmp = shmat(shmid, NULL, 0);//shmid共享内存的ID shmaddr共享内存的起始地址 shmflag本进程对该内存的操作模式
}

int main(){
    my_init();
    int flag=0;

    while(1){
        sem_wait(sem_empty);//将信号量的值减去一个"1"
        sem_wait(sem_mutex);

        if(flag){
            char recv[100];
            strcpy(recv, (char *)shmp);
            printf("respond: %s\n", recv);
            break;
        }

        printf("send>");
        char send[100];
        scanf("%s", send);
        if(strcmp(send,"exit")==0)
            flag=1;
        memset((char *)shmp, '\0', 1024);//初始化内存空间 
        strcpy((char *)shmp, send);//将send内容拷贝至内存空间 

        sem_post(sem_mutex);//给信号量的值加上一个"1"
        sem_post(sem_full);
    }
    sem_unlink("empty");//从系统中删除信号量 
    sem_unlink("full");
    sem_unlink("mutex");
    shmdt(shmp);//将共享内存从当前进程中分离
    shmctl(shmid, IPC_RMID, NULL);//删除共享内存段shmid  cmd删除  buf
    printf("quit sending!\n");
    exit(0);
}