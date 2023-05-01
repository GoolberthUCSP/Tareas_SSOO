#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

int main()
{
    // Receive the message from the sender
    // msg= operation sign + number
    int msqid;
    int rand_num;
    key_t key= 1200, key2= 1234;
    struct msgbuf buff;
    char op;
    size_t buflen;
    int msgflg = IPC_CREAT | 0666;
    if ((msqid = msgget(key, msgflg)) < 0)
      die("msgget()");
    //Receive an answer of message type 1.
    if (msgrcv(msqid, &buff, MAXSIZE, 1, 0) < 0)
      die("msgrcv");

    //Close the queue and delete it
    if (msgctl(msqid, IPC_RMID, NULL) < 0)
      die("msgctl");

    buflen= strlen(buff.mtext) + 1 ;
    op= buff.mtext[0];
    rand_num= atoi(buff.mtext+1);
    if(op=='+') rand_num+=3;
    else if(op=='-') rand_num-=7;
    else if(op=='*') rand_num= rand_num*6;
    else if(op=='/') rand_num= rand_num/3;

    buff.mtype= 1;
    //Resend the msg to the other process with the new number and key2
    // msg= operation sign + number
    if ((msqid = msgget(key2, msgflg)) < 0)
      die("msgget");

    sprintf(buff.mtext, "%c%d", op, rand_num);

    if (msgsnd(msqid, &buff, buflen, IPC_NOWAIT) < 0)
      die("msgsnd");
    return 0;
}
