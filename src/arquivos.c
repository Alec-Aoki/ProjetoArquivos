//
// Created by alecc on 4/4/25.
//

#include <stdio.h>
#include <string.h>
#include "arquivos.h"

/*Armazena os campos do header que sofrem alterações*/
struct arquivo_{
    char status; // Consistência do arquivo de dados. 0 = inconsistente, 1 = consistente
    long int topo; // Byteoffset do primeiro registro logicamente removido. -1 = nenhum registro removido
    long int proxByteOffset; // Valor do próximo byteoffset disponível
    int nroRegArq; // Quantidade de registros não removidos
    int nroRegRem; // Quantidade de registros removidos
};

/*Cria um arquivo binário vazio, cujo nome será a string passada pelo ponteiro nomeArquovo, e retorna um ponteiro para ele*/
FILE* arquivo_criar(char* nomeArquivo){
    FILE* pontArq; // Ponteiro para o arquivo
    
    pontArq = fopen(nomeArquivo, "w+b"); // Cria um arquivo binário para gravação. Caso já exista, sobrescreve
    if(pontArq == NULL){
        printf("Erro ao criar arquivo\n");

        return NULL;
    }

    fclose(pontArq);

    return pontArq;
}