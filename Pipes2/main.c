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
	int rand_num= 0, n_b;
	char buff[SZ];
	char str[SZ];	
	const char *fn= "/tmp/myfifo";
	while(1){
		signal(SIGINT, sighandler);
		if(s== 1){
			rand_num= rand()%1000;
       			int     fd[2], fd_f;
       			pid_t   id_odd, id_even;
        		id_even= fork();
			if(id_even == 0){
                		//Even
                		if(rand_num && !rand_num%2){
					close(fd[0]);
                			n_b= read(fd[0], buff, SZ);
					buff[n_b]= 0;
					printf("Even: %s\n", buff);
					mkfifo(fn, 0666);
					fd_f= open(fn, O_WRONLY);
					write(fd_f, buff, strlen(buff)+1);
                			close(fd_f);
				}
				return 0;
        		}
        		else{
				id_odd= fork();
			       	if (id_odd == 0){
					//Odd
					if(rand_num && rand_num%2){
						close(fd[0]);
						n_b= read(fd[0], buff, SZ);
						buff[n_b]= 0;
						printf("Odd: %s\n", buff);
						mkfifo(fn, 0666);
						fd_f= open(fn, O_WRONLY);
						write(fd_f, buff, strlen(buff)+1);
						close(fd_f);
					}
					return 0;
				}
				else{
					//Parent
					rand_num= rand()%1000;
					pipe(fd);
					close(fd[1]);
					sprintf(str, "%d", rand_num);
					printf("%s\n", str);
					write(fd[1], str, strlen(str)+1);
				}
			}
			s= 0;
		}
	}
        return(0);
}
