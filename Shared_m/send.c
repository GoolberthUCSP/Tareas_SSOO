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

int main(){
    signal(SIGINT, sig_handler);
    srand(time(NULL));
    int shmid;
    int rand_num;
    char oper[5]= {'+','-','*','/'};
    char rand_oper;
    key_t key= 1234;
	char *ptr, *r_num;
    while(true){
        if (s){
            rand_oper= oper[rand()%4];
            rand_num= rand()%1000;
            //Send message
            sprintf(buff.mtext, "%c%d", rand_oper, rand_num);
            if((shmid = shmget(key, MAXBUF, IPC_CREAT | 0666)) < 0){
                died("shmget");
            }
            if((ptr = (char *)shmat(shmid, NULL, 0)) == (char *) -1){
                died("shmat");
            }
            sprintf(r_num, "%d\0", rand_num);
            while(*ptr=='#') sleep(1);
            //Block other process
            *ptr= '#';
            int i=1;
            for (; r_num[i]!='\0'; i++)
                *(ptr+i)= *r_num[i];
            *(ptr+i)= NULL;  
            printf("sent: %c %d\n", rand_oper, rand_num);
            s= false;
            //Unblock other process
            *ptr= rand_oper;
        }
    }


}
