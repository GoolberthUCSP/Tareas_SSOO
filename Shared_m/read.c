#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<string.h>

#define MAXBUF 128
bool s= true;

void died(char *e){
    perror(e);
    exit(1);
}

struct buffer{
    long mtype;
    char mtext[MAXBUF];
};

int main(int argc, char *argv[]){
    //Receive message and print in format N_execution - operation - number
    if (argc != 2) return 1;
    char *n_exec= argv[1];
    key_t key= 1200;
    char *ptr;
    char op;
    int num;
    int shmid;
    if ((shmid = shmget(key, MAXBUF, IPC_CREAT | 0666)) < 0){
        died("shmget");
    }
    //Message = operation + number
    if ((ptr = (char *)shmat(shmid, NULL, 0)) == (char *) -1){
        died("shmat");
    }
    while(*ptr=='#') sleep(1);
    //block shared memory
    *ptr= '#';
    op= *ptr;
    num= atoi(ptr+1);
    printf("%d\t%c\t%d\n", n_exec, op, num);
    return 0;
    //Unblock shared memory
    *ptr= op;
}