#include<fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include<signal.h>
#include <time.h>
#include<sys/stat.h>

const int SZ= 256;

int s= 0;

void sighandler() {
   s= 1;
}

int main(void)
{
	srand(time(0));
	int rand_num, n_b;
	char buff[SZ];
	char str[SZ];	
	const char *fn= "/tmp/myfifo";
	while(1){
		signal(SIGINT, sighandler);
		if(s== 1){
			rand_num= rand()%1000;
       			int     fd[2], fd_f;
       			pid_t   childpid1= -1;
			pid_t	childpid2= -1;
        		pipe(fd);//fd[0]= read fd[1]= write
			close(fd[1]);
			n_b= sprintf(str, "%d", rand_num);
			str[n_b]= 0;
			write(fd[1], str, strlen(str)+1);
			if (rand_num%2) childpid2= fork();
			else childpid1= fork();
        		if(childpid1 == 0){
                		//Even
                		close(fd[0]);
                		n_b= read(fd[0], buff, SZ);
				buff[n_b]= 0;
				mkfifo(fn, 0666);
				fd_f= open(fn, O_WRONLY);
				printf("%s", buff);
				write(fd_f, buff, strlen(buff)+1);
                		return 0;
        		}
        		else if (childpid2 == 0){
				//Odd
				close(fd[0]);
				n_b= read(fd[0], buff, SZ);
				buff[n_b]= 0;
				mkfifo(fn, 0666);
				return 0;
			}
			else if(childpid1+childpid2==-2){
				printf("Error fork\n");
				exit(1);
			}
			s= 0;
		}
	}
        return(0);
}
