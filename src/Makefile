all: compile
	gcc main.o -o main -std=c99 -Wall
	rm *.o

compile:
	gcc -c main.c -o main.o

run:
	./main

clean:
	rm -f main