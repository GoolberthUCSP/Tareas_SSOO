#include<stdlib.h>
#include<stdio.h>

float lib1(void);
float lib2(void);
double lib3(void);

int main(){
	printf("First lib: %f\nSecond lib: %f\nThird lib: %g\n", lib1(), lib2(), lib3());
	return 0;
}
