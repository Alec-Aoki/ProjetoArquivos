all: compile
	gcc auxiliar.o header.o dados.o main.o -o main -std=c99 -Wall -g
	rm *.o

compile:
	gcc -c ./libs/auxiliar/impl/auxiliar.c -o auxiliar.o -g
	gcc -c ./libs/registros/impl/header.c -o header.o -g
	gcc -c ./libs/registros/impl/dados.c -o dados.o -g
	gcc -c main.c -o main.o -g

run:
	./main

clean:
	rm -f main
	rm -f *.bin
	rm -f *.txt