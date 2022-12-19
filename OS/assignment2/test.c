#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#define __NR_mysyscall 333
int main()  {
    printf("123\n");
    int length=15;

    syscall(333,length);
    printf("456\n");
}

