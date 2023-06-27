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
#include <fcntl.h>
#include <sys/stat.h>
#include <thread>
#include <mutex>

#define MAXBUF 256
#define KEY_SM 0x1111
#define KEY_SEM 0x2222
#define device "/dev/ucsp"

static char buffer[MAXBUF]= {0};
static char *ptr;

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

    if (n!=2) die("Invalid number of arguments", 1);
    
    int prg= atoi(args[1]);
    int id_sm= shmget(KEY_SM, MAXBUF, IPC_CREAT | 0666);
    int id_sem= semget(KEY_SEM, 1, 0666 | IPC_CREAT);
    
    if (id_sem < 0) die("semget", 11);
    if (id_sm < 0) die("shmget", 12);

    if((ptr = (char *)shmat(id_sm, NULL, 0)) == (char *) - 1) die("shmat", 13);
    
    memset(ptr, '0', MAXBUF);
    int fd = open(device, O_RDWR);
    if (fd < 0) die("open", 21);
    
    std::thread reader([&](){
        while (true){
            
            if (*(ptr+prg) == '0') usleep(10000);
      
            else{

                if (semop(id_sem, &p, 1) < 0){
                    perror("semop p");
                    exit(13);
                }

                
                int n= read(fd, buffer, MAXBUF);
                if (n<0) die("read", 23);
                printf("%s\n", buffer);
                memset(buffer, '\0', MAXBUF);
                *(ptr+prg)= '0';
                

                if (semop(id_sem, &v, 1) < 0){
                    perror("semop p");
                    exit(14);
                }
  
            }
        }
    });

    std::thread writer([&](){
        while (true){

            scanf("%s", buffer);

            if (semop(id_sem, &p, 1) < 0){
                perror("semop p");
                exit(13);
            }


            int n= write(fd, buffer, strlen(buffer));
            if (n<0) die("write", 22);
            memset(buffer, 0, MAXBUF);
            memset(ptr, '1', MAXBUF);
            *(ptr+prg)= '0';
            

            if (semop(id_sem, &v, 1) < 0){
                perror("semop p");
                exit(14);
            }

        }
    });

    reader.join();
    writer.join();
    
}