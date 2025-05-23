all: compile
	gcc auxiliar.o header.o dados.o teste.o -o teste -std=c99 -Wall -g
	rm *.o

compile:
	gcc -c ./auxiliar.c -o ./auxiliar.o -g
	gcc -c ./registros/impl/header.c -o ./header.o -g
	gcc -c ./registros/impl/dados.c -o ./dados.o -g
	gcc -c teste.c -o teste.o -g

run:
	./teste

clean:
	rm -f teste
	rm -f *.bin
	rm -f *.txt