#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#define MAXSIZE     128

int s= 0;

void die(char *s)
{
  perror(s);
  exit(1);
}

void sighandler(int signum)
{
  s= 1;
}

struct msgbuf
{
  long    mtype;
  char    mtext[MAXSIZE];
};


int main()
{
  signal(SIGINT, sighandler);
  srand(time(NULL));
  char oper[5]= {'+','-','*','/'};
  int rand_num;
  int msqid;
  char rand_oper;
  key_t key= 1200;
  size_t buflen;
  int msgflg = IPC_CREAT | 0666;
  struct msgbuf sbuf;
  while(1){
    if (s==1){
      rand_num= rand()%1000;
      rand_oper= oper[rand()%4];
      if ((msqid = msgget(key, msgflg)) < 0)
        die("msgget()");

      sbuf.mtype = 1;
      sprintf(sbuf.mtext, "%c%d", rand_oper, rand_num);
      buflen= strlen(sbuf.mtext) + 1 ;

      if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0)
        die("msgsnd");
      else
        printf("Message sent\n");
      s= 0;
    }
  }
}
