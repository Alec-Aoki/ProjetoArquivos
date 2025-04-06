#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./libs/arquivos.h"


int main(void){
    char nomeArquivoBin[100];
    char nomeArquivoCSV[100];

    scanf("%s", nomeArquivoBin);
    scanf("%s", nomeArquivoCSV);

    arquivo_criar(nomeArquivoBin, nomeArquivoCSV);

    return 0;
}