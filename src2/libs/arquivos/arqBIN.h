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

/*arqBIN_procurar_dado():
Procura um dado que satisfaz os campos num arquivo .bin
Parâmetro: ponteiro para arquivo
*/
// void arqBIN_procurar_dado(FILE *pontArqBIN);

/*arqBIN_imprimir():
Imprime um arquivo.bin
Parâmetro: ponteiro para arquivo
*/
void arqBIN_imprimir(FILE *pontArqBIN);

/*arqBIN_buscar_dado():
Faz uma busca num arquivo .bin de acordo com a struct busca passada
Parâmetro: ponteiro para arquivo, ponteiro para struct busca
Retorna: booleano (true se encontrado, falso senão)
*/
bool arqBIN_buscar_dado(FILE *pontArqBIN, BUSCA *busca);

#endif