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

#define MAXBUF 256
#define KEY_SM 0x1111
#define device "/dev/ucsp"

static char buffer[MAXBUF];
static char *ptr;

void die(char e[], int n_e){
    perror(e);
    exit(n_e);
}

int main(int n, char *args[]){
    if (n!=2) die("Invalid number of arguments", 1);
    int prg= atoi(args[1]);
    int id_sm= shmget(KEY_SM, MAXBUF, IPC_CREAT | 0666);
    
    if (id_sm < 0)
    {
        die("shmget", 12);
    }
    if((ptr = (char *)shmat(id_sm, NULL, 0)) == (char *) -1){
        die("shmat", 13);
    }
    
    memset(ptr, 0, MAXBUF);
    int fd = open(device, O_RDWR);
    if (fd < 0) die("open", 21);
    
    std::thread reader([&](){
        while (true){
            if (*(ptr+prg)=='0'){
                usleep(10000);
            }
            else{
                
                int n= read(fd, buffer, MAXBUF);
                if (n<0) die("read", 23);
                printf("> %s\n", buffer);
                memset(buffer, '\0', MAXBUF);
                *(ptr+prg)= '0';
                
            }
        }
    });

    std::thread writer([&](){
        while (true){
            printf("< ");
            scanf("%s\0", buffer);

            int n= write(fd, buffer, strlen(buffer));
            if (n<0) die("write", 22);
            memset(buffer, '\0', MAXBUF);
            memset(ptr, '1', MAXBUF);
            *(ptr+prg)= '0';
            printf("Message sent\n");
            
        }
    });

    reader.join();
    writer.join();
    
}