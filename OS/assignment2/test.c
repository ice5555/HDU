#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#define __NR_mysyscall 333
int main()  {
    unsigned int length=10;
    //const char * path = "/home/weiwei/test.txt";
    syscall(__NR_mysyscall,length);
}

