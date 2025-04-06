#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "registros.h"
#include "arquivos.h"

// EM CONSTRUÇÃO
bool arquivo_criar(char* nomeArquivo){
    if(nomeArquivo == NULL){
        printf("Erro com o ponteiro para o nome do arquivo\n");
        return false;
    }

    HEADER* headerArq = header_criar();
    if(headerArq == NULL){
        printf("Erro ao criar header\n");
        return false;
    }

    header_escrever(nomeArquivo, headerArq); // Escreve o header criado no arquivo

    header_apagar(&headerArq);
    return true;
}