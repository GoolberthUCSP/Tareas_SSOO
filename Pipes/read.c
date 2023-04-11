

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>


#define MAX_BUF 1024

int main(int argc, char *argv[])
{
	if(argc!=2){
		printf("Bad arguments.\n");
		exit(1);
	}
	if(argv[1][0]=='o' || argv[1][0]=='e'){}
	else{ 
		printf("Bad arguments.\n");
		exit(1);
	}
    	int fd;	
    	char * myfifo = argv[1];
    	char buf[MAX_BUF];
       	fd = open(myfifo, O_RDONLY);
    	read(fd, buf, MAX_BUF);
    	printf("Received: %s   %d is %s.\n", buf, atoi(buf)+10, myfifo);
    	close(fd);

    return 0;
}


