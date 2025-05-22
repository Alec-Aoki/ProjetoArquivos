#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./libs/arquivos.h"

/*
ALEC CAMPOS AOKI - 15436800
JÕAO RICARDO DE ALMEIDA LUSTOSA - 15463697
*/

int main(void)
{
    char op;
    char nomeArquivoBin[100];
    char nomeArquivoCSV[100];

    scanf("%c", &op);

    switch (op)
    {
    case '1': // Funcionalidade 1
        scanf("%s", nomeArquivoCSV);
        scanf("%s", nomeArquivoBin);
        arquivo_criar(nomeArquivoBin, nomeArquivoCSV);
        break;
    case '2': // Funcionalidade 2
        scanf("%s", nomeArquivoBin);
        arquivo_imprimir(nomeArquivoBin);
        break;
    case '3': // Funcionalidade 3
        scanf("%s", nomeArquivoBin);
        int quantBuscas;
        scanf("%d", &quantBuscas);
        arquivo_buscar(nomeArquivoBin, quantBuscas);
        break;
    }

    return 0;
}