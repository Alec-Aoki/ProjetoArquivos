all: compile
	gcc auxiliar.o header.o dados.o busca.o arqCSV.o arqBIN.o arvB.o funcionalidades.o main.o -o main -std=c99 -Wall -g
	rm *.o

compile:
	gcc -c ./libs/auxiliar/impl/auxiliar.c -o auxiliar.o -g
	gcc -c ./libs/registros/impl/header.c -o header.o -g
	gcc -c ./libs/registros/impl/dados.c -o dados.o -g
	gcc -c ./libs/registros/impl/busca.c -o busca.o -g
	gcc -c ./libs/arquivos/impl/arqCSV.c -o arqCSV.o -g
	gcc -c ./libs/arquivos/impl/arqBIN.c -o arqBIN.o -g
	gcc -c ./libs/arquivos/impl/arvB.c -o arvB.o -g
	gcc -c ./libs/arquivos/impl/funcionalidades.c -o funcionalidades.o -g
	gcc -c main.c -o main.o -g

run:
	./main

clean:
	rm -f main
	rm -f *.txt
	rm -f *.bin

zip:
	zip -r programaTrab.zip ./Makefile ./main.c ./libs/registros/dados.h ./libs/registros/header.h ./libs/registros/busca.h ./libs/registros/impl/dados.c ./libs/registros/impl/header.c ./libs/registros/impl/busca.c ./libs/arquivos/arqCSV.h ./libs/arquivos/arqBIN.h ./libs/arquivos/funcionalidades.h ./libs/arquivos/impl/arqBIN.c ./libs/arquivos/impl/arqCSV.c ./libs/arquivos/impl/funcionalidades.c ./libs/auxiliar/auxiliar.h ./libs/auxiliar/impl/auxiliar.c ./libs/arquivos/arvB.h ./libs/arquivos/impl/arvB.c