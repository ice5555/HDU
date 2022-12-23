#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void childprocess(int num,char * const argv[],char ** environ){
        pid_t pid=fork();
        if(pid<0){
                printf("Create child process failed \n");
        }
        else if(pid==0){
                switch(num){
                        case 1:execve("cmd1",argv,environ);
                        case 2:execve("cmd2",argv,environ);
                        case 3:execve("cmd3",argv,environ);
                        default:printf("process will never go here");
                }
        }
}
int main(int argc,char * const argv[], char **environ){
        char a[10];
        while(1){//由于指令允许无限次输入，所以shell一定是死循环
                scanf("%s",&a);
 
                if (strcmp(a,"exit")==0) break;
                else if (strcmp(a,"cmd1")==0){
                        childprocess(1,argv,environ);
                }
                else if (strcmp(a,"cmd2")==0){
                        childprocess(2,argv,environ);
                }
                else if (strcmp(a,"cmd3")==0){
                        childprocess(3,argv,environ);
                }
                else printf("Command not found\n");
        }
}
