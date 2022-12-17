#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#define __NR_mysyscall 333
int main()  {
    printf("123");
    unsigned int length=10;
    //const char * path = "/home/weiwei/test.txt";
    syscall(333,length);
    printf("456");
}

