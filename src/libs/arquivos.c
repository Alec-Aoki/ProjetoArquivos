#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "registros.h"
#include "arquivos.h"

bool arquivo_criar(char* nomeArqBin, char* nomeArqCSV){
    // Abre o arquivo .csv para leitura
    FILE* pontArqCSV = fopen(nomeArqCSV, "r");
    if(pontArqCSV == NULL){
        printf("Erro ao abrir arquivo .csv\n");
        return false;
    }
    
    /* LEITURA DOS CAMPOS DO HEADER */
    char *camposHeader[7]; // Vetor de ponteiros de strings para guardar os campos do header
    char bufferHeader[254] = ""; // Buffer para leitura do header do .csv
    
    fseek(pontArqCSV, 0, SEEK_SET); // Posiciona o ponteiro no inicio do arquivo
    
    fread(bufferHeader, 253, 1, pontArqCSV); // Lê a primeira linha do .csv (descrições semânticas do header)
    bufferHeader[253] = '\0'; // Substituindo '\n' por '\0' no buffer, por segurança

    // Vamos utilizar o strtok para separar a string no buffer nas ',' e evitar alocar espaço para as strings do header
    char *tokHeader = strtok(bufferHeader, ",");
    // Guarda cada parte da string em um dos campos do vetor camposHeader
    int i = 0;
    while(tokHeader != NULL && i < 7){
        camposHeader[i] = tokHeader;
        tokHeader = strtok(NULL, ",");
        i++;
    }

    // Criando struct header com os campos semânticos do header do .csv
    HEADER *headerArq = header_criar(camposHeader[0], camposHeader[1], camposHeader[2], camposHeader[3], camposHeader[4], camposHeader[5], camposHeader[6]);
    if(headerArq == NULL){
        printf("Erro ao criar header\n");
        return false;
    }
    
    if(nomeArqBin == NULL){
        printf("Erro com o ponteiro para o nome do arquivo\n");
        return false;
    }

    // Cria um arquivo binário para gravação. Caso já exista, sobrescreve
    FILE* pontArqBin = fopen(nomeArqBin, "wb");
    if(pontArqBin == NULL){
        printf("Erro ao criar arquivo\n");
        return false;
    }

    /* HEADER INICIAL */
    header_escrever(pontArqBin, headerArq, true); // Escreve o header criado no arquivo

    /* ATUALIZANDO CAMPOS DO HEADER */
    // Alterando o status do arquivo antes de fechá-lo
    header_set_status(headerArq, '1');
    // ... (outros campos a serem atualizados)

    /* ESCREVENDO HEADER ATUALIZADO NO ARQUIVO */
    header_escrever(pontArqBin, headerArq, false); // Não precisamos escrever os campos semânticos novamente

    /* DESALOCANDO MEMÓRIA E FECHANDO O ARQUIVO */
    header_apagar(&headerArq); // Desalocando struct header
    fclose(pontArqBin); // Fechando o arquivo

    return true;
}