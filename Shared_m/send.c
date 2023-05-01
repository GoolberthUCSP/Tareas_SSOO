#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<signal.h>
#include<string.h>
#include<time.h>
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

int main(){
    signal(SIGINT, sig_handler);
    srand(time(NULL));
    int shmid;
    int rand_num, r_op;
    char oper[5]= {'+','-','*','/'};
    char rand_oper;
    key_t keys[4]= {1201, 1202, 1203, 1204};
    key_t key;
	char r_num[MAXBUF];
    char *ptrs[4], *ptr;
    for (int i=0; i<4; i++){
        if((shmid = shmget(keys[i], MAXBUF, IPC_CREAT | 0666)) < 0){
            died("shmget");
        }
        if((ptrs[i] = (char *)shmat(shmid, NULL, 0)) == (char *) -1){
            died("shmat");
        }
        //Block the shared_memory
        *ptrs[i]= '\0';
        *(ptrs[i]+1)= '\0';
    }
    while(true){
        if (s){
            r_op= rand()%4;
            ptr= ptrs[r_op];
            while(*ptr=='#'){
                sleep(1);
                printf("Waiting reader...\n");
            }
            *ptr= '#'; //Block other process
            rand_oper= oper[r_op];
            rand_num= (rand()%900) + 100;
            key= keys[r_op];
            //Send message
            sprintf(r_num, "%d", rand_num);
            //copy r_num to ptr from position 1 and add '\0' at the end
            strcpy(ptr+1, r_num);
            *ptr= rand_oper; //Unblock other process
            s= false;
            printf("Sent: %s to Sm ID: %d\n", ptr, key);
        }
    }


}
