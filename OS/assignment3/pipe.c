#include <sys/sem.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/wait.h>

#define READ 0  //描述符
#define WRITE 1
#define MAX_SIZE 16384
/*16KB, 当管道进行写入操作的时候，如果写入的数据小于128K则是非原子的，
如果大于128K字节，缓冲区的数据将被连续地写入*/
 
int main(int argc, char** argv){
    int fd[2];
    ssize_t status;
    int flag=0;
    char buf[MAX_SIZE], str[MAX_SIZE];
    pid_t pid1,pid2,pid3;
    //定义三个互斥信号量指针
	sem_t *write_mutex;
	sem_t *read_mutex1;
	sem_t *read_mutex2;
	//创建有名信号量
	write_mutex = sem_open("pipe_test_wm", O_CREAT | O_RDWR, 0666, 0);
	read_mutex1 = sem_open("pipe_test_rm_1", O_CREAT | O_RDWR, 0666, 0);
	read_mutex2 = sem_open("pipe_test_rm_2", O_CREAT | O_RDWR, 0666, 0);
    status = pipe(fd);  
    /*返回值：成功，返回0，否则返回-1。
    参数数组包含pipe使用的两个文件的描述符。fd[0]:读管道，fd[1]:写管道。*/
    if(status < 0){
        fprintf(stderr, "create Pipe Error %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    status = 0;
    //创建子进程1, 测试管道默认大小
	if((pid1 = fork()) == -1){
		printf("child process 1 fork() failed.\n");
		exit(1); //异常退出
	}
	if (pid1 == 0) {
		printf("\n>> child process 1 [pid=%d, ppid=%d]:\n", getpid(), getppid());
		int count = 0;
		close(fd[READ]);  //关闭读端口
		//F_GETFL,获取pipefd[1]的文件状态标志
		int flags = fcntl(fd[WRITE], F_GETFL);
		//F_SETFL,设置pipefd[1]的文件状态标志 非阻塞方式
		fcntl(fd[WRITE], F_SETFL, flags | O_NONBLOCK);
		while (!flag) {
			//write把buf指针所指内存的16384个字节写入管道中，返回实际写入的字节数
			status = write(fd[WRITE], buf, MAX_SIZE);
			//管道已满，不能写入
			if (status == -1) {
				flag = 1;
			} else {
				count++; //记录写入的次数
				printf("child process 1 write %ldB.\n", status);
			}
		}
		//输出管道的默认大小
		printf("pipe size = %dKB.\n\n", (count * MAX_SIZE) / 1024);
		exit(0);
	}

    status = 0;
    //创建子进程2
	if((pid2 = fork()) == -1){
		printf("child process 2 fork() failed.\n");
		exit(1); //异常退出
	}
	if (pid2 == 0) {
		sem_wait(write_mutex);
		close(fd[READ]);
		printf(">> child process 2 [pid=%d, ppid=%d]:\n", getpid(), getppid());
		status = write(fd[WRITE], "This is child process 2.\n", strlen("This is child process 2.\n"));
		printf("child process 2 write %ldB.\n\n", status);
		sem_post(write_mutex);
		sem_post(read_mutex1);
		exit(0);
	}

    status = 0;
	//创建子进程3
	if((pid3 = fork()) == -1){
		printf("child process 3 fork() failed.\n");
		exit(1); //异常退出
	}
	if (pid3 == 0) {
		sem_wait(write_mutex);
		close(fd[READ]);
		printf(">> child process 3 [pid=%d, ppid=%d]:\n", getpid(), getppid());
		status = write(fd[WRITE], "This is child process 3.\n", strlen("This is child process 3.\n"));
		printf("child process 3 write %ldB.\n\n", status);
		sem_post(write_mutex);
		sem_post(read_mutex2);
		exit(0);
	}

	//父进程
	wait(0);
	close(fd[WRITE]);
	printf(">> father process [pid=%d]:\n", getpid());
	int flags = fcntl(fd[READ], F_GETFL);
	fcntl(fd[READ], F_SETFL, flags | O_NONBLOCK);
	while (!flag) {
		//read把管道中的MAX_SIZE个字节读到str指针所指的内存中
		status = read(fd[READ], str, MAX_SIZE);
		if (status == -1) {
			flag = 1;
		} else {
			//输出读出的字节数
			printf("father process read %ldB.\n", status);
		}
	}
	printf("\n");
	sem_post(write_mutex);
	//子进程23全部写完后开始读
	sem_wait(read_mutex1);
	sem_wait(read_mutex2);
	printf(">> father process [pid=%d]:\n", getpid());
	status = read(fd[READ], str, MAX_SIZE);
	//输出子进程23写入的内容
	printf("father process read %ldB.\n", status);
	for (int i = 0; i < status; i++) {
	    printf("%c", str[i]);
	}
	printf("\n");

	//关闭信号量
	sem_close(write_mutex);
	sem_close(read_mutex1);
	sem_close(read_mutex2);
	//从系统中删除信号量
	sem_unlink("pipe_test_wm");
	sem_unlink("pipe_test_rm_1");
	sem_unlink("pipe_test_rm_2");

    return 0;
}
