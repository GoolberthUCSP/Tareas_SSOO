#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<signal.h>
#include<string.h>
#include <unistd.h>
#include <sys/sem.h>

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

int main(){
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
    *ptr= 'U';
    union semun u;
    u.val= 1;
    if (semctl(id_sem, 0, SETVAL, u) < 0)
    {
        die("semctl", 14);
    }
}