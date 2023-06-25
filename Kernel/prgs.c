#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH];

int main(){
   int ret, fd;
   char stringToSend[BUFFER_LENGTH];
   printf("Starting device test code example...\n");
   fd = open("/dev/ebbchar", O_RDWR);
    if (fd < 0){
        perror("Failed to open the device...");
        return errno;
    }
    //TODO: Read from the device
}