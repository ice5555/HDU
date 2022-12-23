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
    sem_empty = sem_open("empty", O_CREAT, 0666, 1);
    sem_full = sem_open("full", O_CREAT, 0666, 0);
    sem_mutex = sem_open("mutex", O_CREAT, 0666, 1);

    shmid = shmget(0x0127, 1024, 0666|IPC_CREAT);
    shmp = shmat(shmid, NULL, 0);//shmid shmaddr shmflag
}

int main(){
    my_init();
    int flag=0;
    while(1){
        sem_wait(sem_full);
        sem_wait(sem_mutex);

        char recv[100];
        strcpy(recv, (char *)shmp);
        printf("receive>%s\n",recv);

        if(strcmp(recv, "exit") == 0){
            char send[100]="over";
            memset((char *)shmp, '\0', 1024);
            strcpy((char *)shmp, send);
            flag=1;
        }

        sem_post(sem_mutex);
        sem_post(sem_empty);

        if(flag) break;
    }
    shmdt(shmp);
    printf("quit receiving!\n");
	exit(0);
}