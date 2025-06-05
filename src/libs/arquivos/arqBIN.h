/*
Lida com o arquivo .bin
*/

#ifndef ARQBIN_H
#define ARQBIN_H

/*arqBin_escrever_header():
Escreve uma struct do tipo header em um arquivo.bin
Parâmetro: ponteiro para arquivo, ponteiro para header, valor booleano
*/
void arqBIN_escrever_header(FILE *pontArqBIN, HEADER *header, bool camposSemanticos);

/*arqBIN_escrever_dado():
Lê um dado de um arquivo .bin
Parâmetro: ponteiro para arquivo, ponteiro para dado
*/
void arqBIN_escrever_dado(FILE *pontArqBIN, DADO *dado);

/*arqBIN_imprimir():
Imprime um arquivo.bin
Parâmetro: ponteiro para arquivo
*/
void arqBIN_imprimir(FILE *pontArqBIN);

/* arqBIN_buscar_byteOffset():
Busca um dado que satisfaz os campos num arquivo .bin e o remove logicamente
Parâmetro: ponteiro para arquivo, ponteiro para struct busca
Retorna: o byteOffset do dado encontrado ou -1 se não encontrado
*/
long int arqBIN_buscar_byteOffset(FILE *pontArqBIN, BUSCA *busca, HEADER *headerArq);

/* arqBIN_delete_dado():
Remove logicamente um dado de um arquivo .bin
Parâmetro: ponteiro para arquivo, ponteiro para struct busca
Retorna: booleano (true se removido, falso senão)
*/
bool arqBIN_delete_dado(FILE *pontArqBIN, BUSCA *busca, HEADER *headerArq);

/* arqBIN_insert_dado():
Insere um dado no arquivo binário, utilizando estratégia de inserção First Fit.
Parâmetro: ponteiro para arquivo, ponteiro para array de strings (entrada)
Retorna: booleano (true se inserido, false se erro)
*/
bool arqBIN_insert_dado(FILE *pontArqBIN, char **entrada);

#endif