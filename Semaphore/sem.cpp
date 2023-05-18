#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>

#define KEY 0x1234
bool s= false;

void signalHandler(int signum){
    s = true;
}

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

int main(int args, char *argv[])
{
    signal(SIGINT, signalHandler);
    if (args != 2)
    {
        printf("Necesary an argument!\n");
        exit(1);
    }
    int n_prog = atoi(argv[1]);
    int id = semget(KEY, 1, 0666 | IPC_CREAT);
    if (id < 0)
    {
        perror("semget");
        exit(11);
    }
    union semun u;
    //El primer programa define el valor del semaforo en 3
    if (n_prog == 1){
        u.val = 3;
        if (semctl(id, 0, SETVAL, u) < 0)
        {
            perror("semctl");
            exit(12);
        }
    }
    if (semop(id, &p, 1) < 0)
    {
        perror("semop p");
        exit(13);
    }
    
    while(true){
        printf("Soy el proceso %d...\n", n_prog);
        fflush(stdout);
        sleep(1);
        if (s){
            break;
        }
    }

    if (semop(id, &v, 1) < 0)
    {
        perror("semop p");
        exit(14);
    }
    printf("Soy el proceso %d y termino.\n", n_prog);
    return 0;
}