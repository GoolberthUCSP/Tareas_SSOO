#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<string.h>

#define MAXBUF 256
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
    struct buffer buff;
    char op;
    char *num;
    int shmid;
    if ((shmid = shmget(key, sizeof(buff), IPC_CREAT | 0666)) < 0){
        died("shmget");
    }
    //Message = operation + number
    if ((buff = (struct buffer *)shmat(shmid, NULL, 0)) == (struct buffer *) -1){
        died("shmat");
    }
    //Get operation
    op= buff->mtext[0];
    //Get number
    num= strtok(buff->mtext, &op);
    //Print in format N_execution - operation - number
    printf("%s \t %c \t %s\n", n_exec, op, num);
    if (shmdt(buff) == -1){
        died("shmdt");
    }
    return 0;
}