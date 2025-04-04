#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "arquivos.h"


int main(void){
    char nomeArquivo[100];

    scanf("%s", nomeArquivo);

    arquivo_criar(nomeArquivo);
}