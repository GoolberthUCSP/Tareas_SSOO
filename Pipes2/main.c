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
	int fd[2], fdfifo;
	pid_t id_odd, id_even;
	const char *fn= "/tmp/myfifo";
	while(1){
		signal(SIGINT, sighandler);
		if(s== 1){
			rand_num= rand()%1000;
        		id_even= fork();
			if(id_even == 0){
                		//Even
                		if(rand_num && !rand_num%2){
					close(fd[0]);
                			n_b= read(fd[0], buff, SZ);
					printf("Even: %s\n", buff);
					mkfifo(fn, 0666);
					fdfifo= open(fn, O_WRONLY);
					write(fdfifo, buff, strlen(buff)+1);
                			close(fdfifo);
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
						printf("Odd: %s\n", buff);
						mkfifo(fn, 0666);
						fdfifo= open(fn, O_WRONLY);
						write(fdfifo, buff, strlen(buff)+1);
						close(fdfifo);
					}
					return 0;
				}
				else{
					//Parent
					rand_num= rand()%1000;
					pipe(fd);
					close(fd[1]);
					sprintf(buff, "%d", rand_num);
					write(fd[1], buff, strlen(buff)+1);
				}
			}
			s= 0;
		}
	}
        return(0);
}
