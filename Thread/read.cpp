#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>

const int SZ=256;
using namespace std;

int main(){
  	const char *ff1= "/tmp/ff1";
    const char *ff2= "/tmp/ff2";
    int fd1, fd2;
    fd1= open(ff1, O_RDONLY);
    fd2= open(ff2, O_RDONLY);
    char buff1[SZ];
    char buff2[SZ];
    read(fd1, buff1, SZ);
    read(fd2, buff2, SZ);
    cout<< "E_received: " << buff1 <<endl;
    cout<< "O_received: " << buff2 <<endl;
    close(fd1);
    close(fd2);
    return 0;
}