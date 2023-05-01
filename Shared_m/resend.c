#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<signal.h>
#include<string.h>
#include <stdbool.h>

#define MAXBUF 20

bool s= false;

void died(char *e){
    perror(e);
    exit(1);
}

void sig_handler(int signo){
    s = true;
}

struct buffer{
    long mtype;
    char mtext[MAXBUF];
};

int main(int n, char *args[]){
    if(n!=2) died("Wrong number of arguments");
    int oper_id= atoi(args[1])%4;
    signal(SIGINT, sig_handler);
    //Receive message and resend to other process
    int shmid;
    key_t keys[4]= {1201, 1202, 1203, 1204};
    key_t key_read= keys[oper_id];
    key_t key_send= 1200;
    char *ptr_read, *ptr_send, rand_num[MAXBUF];
    char oper;
    int a= 1;
    int r_num;
    //Getting shared_memory of the receiver
    if((shmid = shmget(key_read, MAXBUF, IPC_CREAT | 0666)) < 0){
        died("shmget");
    }
    if((ptr_read = shmat(shmid, NULL, 0)) == (char *) -1){
        died("shmat");
    }
    //Getting shared_memory of the sender
    if((shmid = shmget(key_send, MAXBUF, IPC_CREAT | 0666)) < 0){
        died("shmget");
    }
    if((ptr_send = shmat(shmid, NULL, 0)) == (char *) -1){
        died("shmat");
    }
    *ptr_send= '\0';
    while(true){
        if (s){
            while(*ptr_read=='#' || *ptr_read=='\0'){
                sleep(1);
                printf("Waiting sender...\n");
            }
            oper= ptr_read[0];
            *ptr_read= '#';//Block the reader shared_memory
            r_num= atoi(ptr_read+1);
            printf("received: %c\t%s\n", oper, rand_num);
            if (oper=='+') r_num= r_num+7;
            else if (oper=='-') r_num= r_num-5;
            else if (oper=='*') r_num= r_num*2;
            else if (oper=='/') r_num= r_num/2;
            sprintf(rand_num, "%d", r_num);
            *ptr_read= '\0';//Indicate than the reader can read again

            while(*ptr_send=='#'){
                sleep(1);
                printf("Waiting reader...\n");
            }
            *ptr_send= '#';//Block the sender shared_memory
            //copy r_num to ptr_send from position 1 and add '\0' at the end
            strcpy(ptr_send+1, rand_num);
            printf("resent: %c\t%d\n", oper, r_num);
            *ptr_send= oper;//Unblock the sender shared_memory
            s = false;
        }
    }
}