#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<signal.h>
#include<string.h>
#include<map.h>

#define MAXBUF 128
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

int main(int n, char *args[]){
    if(n!=2) exit(1);
    int oper= atoi(args[1]);
    signal(SIGINT, sig_handler);
    //Receive message and resend to other process
    int shmid;
    key_t keys[4]= {1233, 1234, 1235, 1236};
    key_t key= keys[oper];
    key_t key2= 1200;
    char *ptr1, *ptr2, *tmp, *rand_num;
    char op;
    int a= 1;
    int r_num;
    while(true){
        if (s){
            if((shmid = shmget(key, MAXBUF, IPC_CREAT | 0666)) < 0){
                died("shmget");
            }
            //Read message
            if((ptr1 = (char *)shmat(shmid, NULL, 0)) == (char *) -1){
                died("shmat");
            }
            //Send message
            if((shmid = shmget(key2, MAXBUF, IPC_CREAT | 0666)) < 0){
                died("shmget");
            }
            if((ptr2 = (char *)shmat(shmid, NULL, 0)) == (char *) -1){
                died("shmat");
            }
            while(*ptr1=='#') sleep(1);
            op= *ptr1;
            *ptr1= '#';//Block the shared_memory
            tmp= ptr1;
            r_num= atoi(tmp+1);
            if (op=='+') r_num= r_num+7;
            else if (op=='-') r_num= r_num-5;
            else if (op=='*') r_num= r_num*2;
            else if (op=='/') r_num= r_num/3;
            sprintf(rand_num, "%d\0", r_num);
            tmp= ptr2;
            while(*tmp=='#') sleep(1);
            //Block the shared_memory
            *tmp= '#';
            int i=1;
            for (; rand_num[i]!='\0'; i++)
                *(tmp+i)= *rand_num[i];
            printf("resent: %c %d\n", op, r_num)
            s = false;
            *ptr1= op;//Unblock the shared_memory
            *tmp= op;//Unblock the shared_memory
        }
    }
}