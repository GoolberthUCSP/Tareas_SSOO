#include<fcntl.h>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <unistd.h>
#include<random>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

const int SZ=256;

int rand_num;
bool s= false;

char even[10]= "/tmp/even";
char odd[10]= "/tmp/odd";
char *ff[10];

void f_th(int n) 
{
  char buff[SZ];
  if (n==rand_num%2){
    int num= (n==1)? rand_num*5 : rand_num*7;
    *ff= (n==1)? even : odd;
    mkfifo(*ff, 0666);
    int fd= open(*ff, O_WRONLY);  
    sprintf(buff, "%d", num);
    write(fd, buff, strlen(buff)+1);
    close(fd);
    unlink(*ff);
  }
  printf("Rand: %d", rand_num);
}

void th(){
  return;
}

void sighandler(int signum=0){
  s= true;
}

int main() 
{
  srand(time(0));
	signal(SIGINT, sighandler);
  while(1){
	  if (s){
      rand_num= rand()%1000;
      std::thread th1(f_th, 1);
  		std::thread th2(f_th, 0);
  		th1.join();
  		th2.join();
		  s= false;
	  }
  }
  return 0;
}
