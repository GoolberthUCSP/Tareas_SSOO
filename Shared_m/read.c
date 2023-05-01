#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<string.h>
#include <stdbool.h>

#define MAXBUF 20

bool s= true;

void died(char *e){
    perror(e);
    exit(1);
}

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
    while(*ptr=='#' || *ptr=='\0') sleep(1);
    op= *ptr;
    *ptr= '#'; //block shared memory
    num= atoi(ptr+1);
    printf("%s\t%c\t%d\n", n_exec, op, num);
    *ptr= '\0'; //Indicate than the reader has finished
    return 0;
}