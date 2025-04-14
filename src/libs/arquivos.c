#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "registros.h"
#include "arquivos.h"

// EM CONSTRUÇÃO
bool arquivo_criar(char* nomeArqBin, char* nomeArqCSV){

    FILE* pontArqCSV = fopen(nomeArqCSV, "r"); // Abre o arquivo .csv para leitura
    if(pontArqCSV == NULL){
        printf("Erro ao abrir arquivo .csv\n");
        return false;
    }
    
    /* LEITURA DOS CAMPOS DO HEADER */
    char *camposHeader[7]; // Matriz de strings para guardar os campos do header
    char bufferHeader[254] = ""; // Buffer para leitura das strings do campo
    // Posiciona o ponteiro no inicio do arquivo
    fseek(pontArqCSV, 0, SEEK_SET);
    // Lendo os campos do header e guardando na matriz camposHeader
    fread(bufferHeader, 253, 1, pontArqCSV);
    bufferHeader[253] = '\0';
    char *tokHeader = strtok(bufferHeader, ",");
    int i = 0;
    while(tokHeader != NULL && i < 7){
        camposHeader[i] = tokHeader;
        tokHeader = strtok(NULL, ",");
        i++;
    }

    HEADER *headerArq = header_criar(camposHeader[0], camposHeader[1], camposHeader[2], camposHeader[3], camposHeader[4], camposHeader[5], camposHeader[6]);
    if(headerArq == NULL){
        printf("Erro ao criar header\n");
        return false;
    }
    
    if(nomeArqBin == NULL){
        printf("Erro com o ponteiro para o nome do arquivo\n");
        return false;
    }

    FILE* pontArqBin = fopen(nomeArqBin, "wb"); // Cria um arquivo binário para gravação. Caso já exista, sobrescreve
    if(pontArqBin == NULL){
        printf("Erro ao criar arquivo\n");
        return false;
    }

    /*HEADER INICIAL*/
    header_escrever(pontArqBin, headerArq, true); // Escreve o header criado no arquivo

    /*ATUALIZANDO CAMPOS DO HEADER*/
    // Alterando o status do arquivo antes de fechá-lo
    header_set_status(headerArq, '1');
    // ... (outros campos a serem atualizados)

    /*ESCREVENDO HEADER ATUALIZADO NO ARQUIVO*/
    header_escrever(pontArqBin, headerArq, false);

    /*DESALOCANDO MEMÓRIA E FECHANDO O ARQUIVO*/
    header_apagar(&headerArq); // Desalocando struct header
    fclose(pontArqBin); // Fechando o arquivo

    return true;
}