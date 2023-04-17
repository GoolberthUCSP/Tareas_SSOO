#include<fcntl.h>
#include <iostream>       // std::cout
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
bool is_even;

void f_th(bool is_even, const char* ff) 
{
  mkfifo(ff, 0666);
  int fd= open(ff, O_WRONLY);
  char buff[SZ];
  if (is_even==rand_num%2){
    sprintf(buff, "%d", rand_num);
    write(fd, buff, strlen(buff)+1);
  }
  else{
    sprintf(buff, "%s", "No number");
    write(fd, buff, strlen(buff)+1);
  }
  close(fd);
}

void sighandler(int signum=0){
  s= true;
}
 
int main() 
{
  srand(time(NULL));
	signal(SIGINT, sighandler);
  while(1){
	  if (s){
      rand_num= rand(); 
  		const char *ff1= "/tmp/ff1";
  		const char *ff2= "/tmp/ff2";
  		is_even= rand_num%2;
      std::thread th1(f_th, std::ref(is_even), std::ref(ff1));
  		std::thread th2(f_th, std::ref(is_even), std::ref(ff2));
  		th1.join();
  		th2.join();
		  s= false;
	  }
  }
  return 0;
}
