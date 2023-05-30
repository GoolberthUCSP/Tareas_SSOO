#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<signal.h>
#include<string.h>
#include <unistd.h>
#include <sys/sem.h>
#include <map>

#define MAXBUF 10
#define KEY_SM 0x1111
#define KEY_SEM 0x2222

void die(char *e, int n_e){
    perror(e);
    exit(n_e);
}

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

int main(int n, char *args[]){
    int prog= atoi(args[1]), index=1;
    char *data;
    std::map<char, char> next;
    next['U']= 'L';
    next['L']= 'N';
    next['N']= 'U';
    if (prog==1) data= "UABCDEFGHIJ";
    else if (prog==2) data= "Labcdefghij";
    else if (prog==3) data= "N0123456789";
    else die("Invalid program number", 1);
    int id_sem= semget(KEY_SEM, 1, 0666 | IPC_CREAT);
    int id_sm= shmget(KEY_SM, MAXBUF, IPC_CREAT | 0666);
    if (id_sem < 0)
    {
        die("semget", 11);
    }
    if (id_sm < 0)
    {
        die("shmget", 12);
    }
    char *ptr;
    if((ptr = (char *)shmat(id_sm, NULL, 0)) == (char *) -1){
        die("shmat", 13);
    }
    for (;;){
        if (*ptr==data[0]){
            if (semop(id_sem, &p, 1) < 0)
            {
                perror("semop p");
                exit(13);
            }

            putchar(data[index]);
            fflush(stdout);
            usleep(80000);
            putchar(data[index]);
            fflush(stdout);

            *ptr= next[data[0]];
            index++;

            if (semop(id_sem, &v, 1) < 0)
            {
                perror("semop p");
                exit(14);
            }
        }
        else{
            usleep(20000);
        }
        if (index==strlen(data)) break;
    }
    return 0;
}