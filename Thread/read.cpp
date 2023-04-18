#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>

const int SZ=256;

int main(){
  	const char *ff1= "/tmp/even", *ff2= "/tmp/odd";
    int fd1, fd2;
    fd1= open(ff1, O_RDONLY);
    fd2= open(ff2, O_RDONLY);
    char buff1[SZ], buff2[SZ];
    read(fd1, buff1, SZ);
    read(fd2, buff2, SZ);
    printf("Even: %s\nOdd : %s\n", buff1, buff2);
    close(fd1);
    close(fd2);
    return 0;
}