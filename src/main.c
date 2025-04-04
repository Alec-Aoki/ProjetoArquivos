#include <stdio.h>
#include "arquivos.h"


int main(void){
    char nomeArquivo[100];

    scanf("%s", nomeArquivo);

    arquivo_criar(nomeArquivo);
}