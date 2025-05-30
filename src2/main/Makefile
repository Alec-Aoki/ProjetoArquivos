all: compile
	gcc auxiliar.o header.o dados.o arqCSV.o arqBIN.o funcionalidades.o main.o -o main -std=c99 -Wall -g
	rm *.o

compile:
	gcc -c ../libs/auxiliar/impl/auxiliar.c -o auxiliar.o -g
	gcc -c ../libs/registros/impl/header.c -o header.o -g
	gcc -c ../libs/registros/impl/dados.c -o dados.o -g
	gcc -c ../libs/arquivos/impl/arqCSV.c -o arqCSV.o -g
	gcc -c ../libs/arquivos/impl/arqBIN.c -o arqBIN.o -g
	gcc -c ../libs/arquivos/impl/funcionalidades.c -o funcionalidades.o -g
	gcc -c main.c -o main.o -g

run:
	./main

clean:
	rm -f main
	rm -f *.bin
	rm -f *.txt