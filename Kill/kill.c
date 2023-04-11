#include<signal.h>
#include<stdlib.h>
#include<stdio.h>

int main(int argc, char *argv[]){
	if (argc!= 3){
		printf("Need 2 parameters.\n");
		exit(1);
	}
	int type= atoi(argv[1]);
	int pID= atoi(argv[2]);
	kill(pID, type);
	printf("Process killed.\n");
	return 0;
}
