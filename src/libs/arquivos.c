#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "registros.h"
#include "arquivos.h"

// EM CONSTRUÇÃO
bool arquivo_criar(char* nomeArqBin, char* nomeArqCSV){
    if(nomeArqBin == NULL){
        printf("Erro com o ponteiro para o nome do arquivo\n");
        return false;
    }

    FILE* pontArq = fopen(nomeArqBin, "wb"); // Cria um arquivo binário para gravação. Caso já exista, sobrescreve
    if(pontArq == NULL){
        printf("Erro ao criar arquivo\n");
        return false;
    }

    HEADER* headerArq = header_criar();
    if(headerArq == NULL){
        printf("Erro ao criar header\n");
        return false;
    }

    header_escrever(pontArq, headerArq); // Escreve o header criado no arquivo

    header_apagar(&headerArq);
    fclose(pontArq); // Fechando o arquivo

    return true;
}