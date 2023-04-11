#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

const int SZ= 256;

int main(){
	const char *fn= "/tmp/myfifo";
	char buff[SZ];
	int nb, fd;
	fd= open(fn, O_RDONLY);
	nb= read(fd, buff, SZ);
	buff[nb]= 0;
	printf("%s\n", buff);
	return 0;
}
