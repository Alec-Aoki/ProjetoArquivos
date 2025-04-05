#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./libs/registros.h"


int main(void){
    char nomeArquivo[100];

    scanf("%s", nomeArquivo);

    arquivo_criar(nomeArquivo);

    return 0;
}