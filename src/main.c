#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./libs/arquivos.h"


int main(void){
    char op;
    char nomeArquivoBin[100];

    scanf("%c", &op);
    scanf("%s", nomeArquivoBin);

    // Funcionalidade 1
    if(op == '1'){
        char nomeArquivoCSV[100];

        scanf("%s", nomeArquivoCSV);

        arquivo_criar(nomeArquivoBin, nomeArquivoCSV);   
    }
        
    // Funcionalidade 2
    else if (op == '2'){
        arquivo_imprimir(nomeArquivoBin);
    }

    return 0;
}