//
// Created by alecc on 4/4/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "arquivos.h"

/*Armazena os campos do header que sofrem alterações*/
struct header_{
    char status; // Consistência do arquivo de dados. 0 = inconsistente, 1 = consistente
    long int topo; // Byteoffset do primeiro registro logicamente removido. -1 = nenhum registro removido
    long int proxByteOffset; // Valor do próximo byteoffset disponível
    int nroRegArq; // Quantidade de registros não removidos
    int nroRegRem; // Quantidade de registros removidos
};

/*FUNÇÕES AUXILIARES*/
/*
Cria uma struct do tipo HEADER e a inicializa
Parâmetros: void
Retorna: ponteiro para a struct do tipo HEADER
*/
HEADER* header_criar(void){
    HEADER* novoHeader = (HEADER *) malloc(sizeof(HEADER)); // Alocando dinâmicamente uma struct do tipo HEADER
    if(novoHeader == NULL){
        printf("Erro ao criar struct\n");

        return NULL;
    }

    // Inicializando a struct
    novoHeader->status = '0';
    novoHeader->topo = -1;
    novoHeader->proxByteOffset = 0;
    novoHeader->nroRegArq = 0;
    novoHeader->nroRegRem = 0;

    return novoHeader;
}

/*
Cria um arquivo binário e o inicializa com o header
Parâmetros: ponteiro para uma string (nome do arquivo a ser criado)
Retorna:
    Caso bem-sucedido: true
    Caso contrário: false
*/
bool arquivo_criar(char* nomeArquivo){
    if(nomeArquivo == NULL){
        printf("Erro com o ponteiro para o nome do arquivo\n");

        return false;
    }

    FILE* pontArq = fopen(nomeArquivo, "wb"); // Cria um arquivo binário para gravação. Caso já exista, sobrescreve
    if(pontArq == NULL){
        printf("Erro ao criar arquivo\n");

        return false;
    }

    HEADER* headerTemp = header_criar();
    if(headerTemp == NULL){
        printf("Erro ao criar struct\n");

        return false;
    }

    fwrite(&(headerTemp->status), sizeof(char), 1, pontArq);

    //char* string = (char *) malloc(67 * sizeof(char)); // Alocando dinâmicamente uma string de tamanho máximo de 67 caracteres
    //strcpy(string, "0");
    //fwrite(string, sizeof(char), 1, pontArq);

    free(headerTemp); // Desalocando struct HEADER criada
    fclose(pontArq); // Fechando o arquivo

    return true;
}