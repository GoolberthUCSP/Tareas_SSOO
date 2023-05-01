#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE     128

void die(char *s)
{
  perror(s);
  exit(1);
}

struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};

int main(int n, char* var[])
{
    if(n!=2) return -1;
    int msqid;
    char *exec= var[1];
    key_t key= 1234;
    struct msgbuf buff;
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0)
      die("msgget()");

     //Receive an answer of message type 1.
    if (msgrcv(msqid, &buff, MAXSIZE, 1, 0) < 0)
      die("msgrcv");
    char op= buff.mtext[0];
    // Iteración Operación Resultdo
    printf("%s \t %c \t %s\n", exec, op, buff.mtext+1);
    return 0;
}
