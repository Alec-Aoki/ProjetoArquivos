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

    /*HEADER INICIAL*/
    header_escrever(pontArq, headerArq, true); // Escreve o header criado no arquivo

    /*DADOS DO ARQUIVOS*/
    // 1. Criar struct registro dado (TAD ARQUIVOS e TAD REGISTRO)
    // 2. Ler campos do csv (TAD ARQUIVOS)
    // 3. Escrever esses campos na struct (TAD arquivos)
    // 4. Escrever essa struct no arquivo (TAD registros)
    // 5. Repetir a partir do passo 2 até o fim do arquivo csv

    /*ATUALIZANDO CAMPOS DO HEADER*/
    // Alterando o status do arquivo antes de fechá-lo
    header_set_status(headerArq, '1');
    // ... (outros campos a serem atualizados)

    /*ESCREVENDO HEADER ATUALIZADO NO ARQUIVO*/
    header_escrever(pontArq, headerArq, false);

    /*DESALOCANDO MEMÓRIA E FECHANDO O ARQUIVO*/
    header_apagar(&headerArq); // Desalocando struct header
    fclose(pontArq); // Fechando o arquivo

    return true;
}