#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>


int main(int argc, char *argv[])
{
	if (argc != 4){
		printf("Bad parameters.\n");
		exit(1);
	}
    	int fd;
    	char * myfifo;
	int a= atoi(argv[1]), b= atoi(argv[3]), c;
	char op= argv[2][0];
	if (op=='+') c= a+b;
	else if (op=='-') c= a-b;
	else if(op=='/') c= a/b;
	else if(op=='*') c= a*b;
	else c=0;
	if(c%2){
		myfifo= "even";
	}else{
		myfifo= "odd";
	}
	mkfifo(myfifo, 0666);
	fd= open(myfifo, O_WRONLY);
	char res[5];
	snprintf(res, 10, "%d", c);
	printf("%s",res);
	write(fd, res, sizeof(res));
    	close(fd);
    	unlink(myfifo);
    	return 0;
}

