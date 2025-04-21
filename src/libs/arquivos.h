#ifndef ARQUIVOS_H
    #define ARQUIVOS_H

    // Transforma um arquivo de .csv para .bin
    void arquivo_criar(char* nomeArqBin, char* nomeArqCSV);

    // Imprime todos os registros de um arquivo binário
    void arquivo_imprimir(char* nomeArqBin);

    /*TODO*/
    void arquivo_buscar(char* nomeArqBin, int quantBuscas);

    void busca(char* nomeArqBin, int quantBuscas);
#endif