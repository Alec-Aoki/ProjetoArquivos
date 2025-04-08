#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "registros.h"
#include "arquivos.h"

// EM CONSTRUÇÃO
bool arquivo_criar(char* nomeArqBin, char* nomeArqCSV){
    /*TRATAMENTO DE ERROS E ABERTURA DO ARQUIVO*/
    if(nomeArqBin == NULL){
        printf("Erro com o ponteiro para o nome do arquivo\n");
        return false;
    }

    FILE* pontArqBin = fopen(nomeArqBin, "wb"); // Cria um arquivo binário para gravação. Caso já exista, sobrescreve
    if(pontArqBin == NULL){
        printf("Erro ao criar arquivo\n");
        return false;
    }


    FILE* pontArqCSV = fopen(nomeArqCSV, "r"); // Abre o arquivo .csv para leitura
    if(pontArqCSV == NULL){
        printf("Erro ao abrir arquivo .csv\n");
        return false;
    }

    HEADER* headerArq = header_criar();
    if(headerArq == NULL){
        printf("Erro ao criar header\n");
        return false;
    }

    /*HEADER INICIAL*/
    header_escrever(pontArqBin, headerArq, true); // Escreve o header criado no arquivo

    /*DADOS DO ARQUIVOS*/
    // 1. Ler campos do csv (TAD ARQUIVOS)
    // 2. Criar struct registro dado (TAD ARQUIVOS e TAD REGISTRO)
    // 3. Escrever esses campos na struct (TAD arquivos)
    // 4. Escrever essa struct no arquivo (TAD registros)
    // 5. Repetir a partir do passo 2 até o fim do arquivo csv

    /*
    char stringTemp[256];
    fread(stringTemp, sizeof(char), 253, pontArqCSV);
    stringTemp[253] = '\0';
    printf("%s\n", stringTemp);

    fread(stringTemp, sizeof(char), 41, pontArqCSV);
    stringTemp[41] = '\0';
    printf("%s\n", stringTemp);
    */

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