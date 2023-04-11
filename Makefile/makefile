cc= gcc
cflags= -Wall
lib_c= lib1.c lib2.c lib3.c
lib_o= lib1.o lib2.o lib3.o
main= main.c


all: $(lib_o)
	$(cc) $(cflags) $(lib_c) -o main.exe $(main) 

run: main.exe
	./main.exe

%.o: %.c
	gcc -c $(cflags) $< -o $@

clean:
	rm -f *.o main.exe
