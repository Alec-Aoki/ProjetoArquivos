/*
Lida com as 3 principais funcionalidades do
projeto, utilizando as outras 2 bilbiotecas
para tanto
*/

#ifndef ARQUIVOS_H
    #define ARQUIVOS_H

    /* arquivo_criar()
    Transforma um arquivo .csv em .bin
    Parâmetros: ponteiros para strings (nomes dos arquivos)
    */
    void arquivo_criar(char* nomeArqBin, char* nomeArqCSV);

    /* arquivo_imprimir():
    Imprime os conteúdos do arquivo binário de acordo com as descrições do header
    Parâmetros: ponteiro para string (nome do arquivo binário)
    */
    void arquivo_imprimir(char* nomeArqBin);

    /* arquivo_buscar():
    Imprime na tela a quantidade requisitada de dados que satisfazer os campos de busca
    Parâmetros: ponteiro para string (nome do arquivo binário), quanttidade de buscas a serem feitas
    */
    void arquivo_buscar(char* nomeArqBin, int quantBuscas);
#endif