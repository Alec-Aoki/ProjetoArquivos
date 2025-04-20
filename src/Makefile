all: compile
	gcc auxiliar.o registros.o arquivos.o main.o -o main -std=c99 -Wall -g
	rm *.o

compile:
	gcc -c ./libs/auxiliar.c -o auxiliar.o -g
	gcc -c ./libs/registros.c -o registros.o -g
	gcc -c ./libs/arquivos.c -o arquivos.o -g
	gcc -c main.c -o main.o -g

run:
	./main

clean:
	rm -f main
	rm -f *.bin
	rm -f *.txt

remake:
	make clean
	make all
	make run