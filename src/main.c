#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./libs/arquivos.h"


int main(void){
    char op;
    char nomeArquivoBin[100];
    char nomeArquivoCSV[100];

    scanf("%c", &op);
        

    // Funcionalidade 1
    if(op == '1'){
        
        scanf("%s", nomeArquivoCSV);
        scanf("%s", nomeArquivoBin);

        arquivo_criar(nomeArquivoBin, nomeArquivoCSV);   
    }
        
    // Funcionalidade 2
    if (op == '2'){
        scanf("%s", nomeArquivoBin);
        arquivo_imprimir(nomeArquivoBin);
    }

    if (op == '3'){
        scanf("%s", nomeArquivoBin);
        int quantBuscas;
        scanf("%d", &quantBuscas);
        busca(nomeArquivoBin, quantBuscas);
    }

    return 0;
}