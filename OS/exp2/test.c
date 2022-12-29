#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#define __NR_mysyscall 333
int main()  {
    
    int time=15;
    printf("即将在 %d 秒后重启\n",time);
    syscall(333,time);
 
}
