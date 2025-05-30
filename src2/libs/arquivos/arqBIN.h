/*
Lida com o arquivo .bin
*/

#ifndef ARQBIN_H
#define ARQBIN_H

/*arqBin_ler_header():
Lê o header de um arquivo .bin e cria uma struct do tipo header com os dados lidos
Parâmetro: ponteiro para arquivo
Retorno: ponteiro para struct do tipo header (NULL se mal sucedido)
*/
HEADER *arqBIN_ler_header(FILE *pontArqBIN);

/*arqBIN_ler_dado():
Lê um dado de um arquivo .bin
Parâmetro: ponteiro para arquivo
Retorno: ponteiro para struct do tipo dado (NULL se mal sucedido)
*/
DADO *arqBIN_ler_dado(FILE *pontArqBIN);

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

#endif