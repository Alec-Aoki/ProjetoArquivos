all: compile
	gcc registros.o main.o -o main -std=c99 -Wall
	rm *.o

compile:
	gcc -c ./libs/registros.c -o registros.o
	gcc -c main.c -o main.o

run:
	./main

clean:
	rm -f main