#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<signal.h>
#include<string.h>

#define MAXBUF 256
bool s= true;

void died(char *e){
    perror(e);
    exit(1);
}

void sig_handler(int signo){
    if(signo == SIGINT){
        s = false;
    }
}

struct buffer{
    long mtype;
    char mtext[MAXBUF];
};

int main(){
    signal(SIGINT, sig_handler);
    //Receive message and resend to other process
    int shmid;
    key_t key= 1234, key2= 1200;
    struct buffer buff1, buff2;
    while(true){
        if (s){
            if((shmid = shmget(key, sizeof(buff1), IPC_CREAT | 0666)) < 0){
                died("shmget");
            }
            //Read message
            if((buff1 = (struct buffer *)shmat(shmid, NULL, 0)) == (struct buffer *) -1){
                died("shmat");
            }
            //Send message
            if((shmid = shmget(key2, sizeof(buff2), IPC_CREAT | 0666)) < 0){
                died("shmget");
            }
            if((buff2 = (struct buffer *)shmat(shmid, NULL, 0)) == (struct buffer *) -1){
                died("shmat");
            }
            buff2->mtype = 1;
            strcpy(buff2->mtext, buff1->mtext);
            if(shmdt(buff1) == -1){
                died("shmdt");
            }
            if(shmdt(buff2) == -1){
                died("shmdt");
            }
            s = false;
        }
    }
}