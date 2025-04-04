all: compile
	gcc arquivos.o main.o -o main -std=c99 -Wall
	rm *.o

compile:
	gcc -c arquivos.c -o arquivos.o
	gcc -c main.c -o main.o

run:
	./main

clean:
	rm -f main