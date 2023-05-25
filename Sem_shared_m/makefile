all:
	g++ -o main main.cpp
	clear

config:
	gcc -o config config.c
	./config
	rm -rf config
	clear

run:
	@./main 1 & ./main 2 & ./main 3 
	@echo "\n"
	@read -p "Press enter to continue..." enter
	rm -rf main
	clear