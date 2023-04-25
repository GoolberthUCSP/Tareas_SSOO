#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<signal.h>
#include<string.h>
#include<random>
#include<time.h>

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
    srand(time(NULL));
    int shmid;
    int rand_num;
    char oper[5]= {'+','-','*','/'};
    char rand_oper= oper[((int)rand()%4)];
    key_t key= 1234;
    struct buffer buff;
    while(true){
        if (s){
            rand_num= rand()%1000;
            //Send message
            sprintf(buff.mtext, "%c%d", rand_oper, rand_num);
            if((shmid = shmget(key, sizeof(buff), IPC_CREAT | 0666)) < 0){
                died("shmget");
            }
            if((buff = (struct buffer *)shmat(shmid, NULL, 0)) == (struct buffer *) -1){
                died("shmat");
            }
            buff->mtype = 1;
            strcpy(buff->mtext, buff.mtext);
            if(shmdt(buff) == -1){
                died("shmdt");
            }
            s = false;
        }
    }


}