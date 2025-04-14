#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "registros.h"

/*Armazena os campos do header que sofrem alterações*/
struct header_{
    char status; // Consistência do arquivo de dados. 0 = inconsistente, 1 = consistente
    long int topo; // Byteoffset do primeiro registro logicamente removido. -1 = nenhum registro removido
    long int proxByteOffset; // Valor do próximo byteoffset disponível
    int nroRegArq; // Quantidade de registros não removidos
    int nroRegRem; // Quantidade de registros removidos
    char descreveIdentificador[23]; // Descrição do campo idAttack
    char descreveYear[27]; // Descrição do campo year
    char descreveFinancialLoss[28]; // Descrição do campo financialLoss
    char codDescreveCountry; // Código da keyword que representa o campo country
    char descreveCountry[26]; // Descrição do campo country
    char codDescreveType; // Código da keyword que representa o campo attackType
    char descreveType[38]; // Descrição do campo attackType
    char codDescreveTargetIndustry; // Código da keyword que representa o campo targetIndustry
    char descreveTargetIndustry[38]; // Descrição do campo targetIndustry
    char codDescreveDefense; // Código da keyword que representa o campo defenseMechanism
    char descreveDefense[67]; // Descrição do campo defenseMechanism
};

/*Armazena os campos de um registro de dados*/
struct dados_ {
    char removido; // Indica se o registro está logicamente removido. 1 = removido, 0 = não removido
    int tamanhoRegistro; // Tamanho do registro em bytes
    long int prox; // Byteoffset do próximo registro logicamente removido. Inicializado com -1
    int idAttack; // Código identificador do ataque
    int year; // Ano em que o ataque ocorreu
    float financialLoss; // Prejuízo causado pelo ataque
    char* country; // País onde ocorreu o ataque
    char* attackType; // Tipo de ameaça à segurança cibernética
    char* targetIndustry; // Setor da indústria que sofreu o ataque
    char* defenseMechanism; // Estratégia de defesa usada para resolver o problema
};

/* header_criar():
Cria uma struct do tipo HEADER e a inicializa
Parâmetros: strings para as descriçõs do header
Retorna: ponteiro para a struct do tipo HEADER
*/
HEADER* header_criar(char* descIdent, char* descYear, char* descFinLoss, char* descCountry, char* descType, char* descTargInd, char* descDef){
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
    novoHeader->codDescreveCountry = 1;
    novoHeader->codDescreveType = 2;
    novoHeader->codDescreveTargetIndustry = 3;
    novoHeader->codDescreveDefense = 4;

    strcpy(novoHeader->descreveIdentificador, descIdent);
    strcpy(novoHeader->descreveYear, descYear);
    strcpy(novoHeader->descreveFinancialLoss, descFinLoss);
    strcpy(novoHeader->descreveCountry, descCountry);
    strcpy(novoHeader->descreveType, descType);
    strcpy(novoHeader->descreveTargetIndustry, descTargInd);
    strcpy(novoHeader->descreveDefense, descDef);

    return novoHeader; // Retornando ponteiro para HEADER
}

/* header_set_status():
Define o campo de status de um header pré-existente
Parâmetros: ponteiro para o header, status a ser definido
Retorna: true se bem sucedido, false senão
*/
bool header_set_status(HEADER* header, char status){
    if(header == NULL){
        printf("Erro ao acessar header\n");
        return false;
    }

    header->status = status; // Definindo o novo status
    return true;
}

/* header_apagar():
Desaloca uma struct do tipo header e define seu ponteiro para NULL
Parâmetros: ponteiro de ponteiro para a struct a ser desalocada
Retorna: void
*/
void header_apagar(HEADER** header){
    free(*header); // Desalocando memória
    *header = NULL; // Definindo o ponteiro para NULL

    return;
}

/* header_escrever():
Escreve um header passado no arquivo binário
Parâmetros: ponteiro para um arquivo, ponteiro para um header e valor booleano (true = escrever string semanticas, false = escrever somente struct)
Retorna:
    Caso bem-sucedido: true
    Caso contrário: false
*/
bool header_escrever(FILE* pontArq, HEADER* headerArq, bool semantico){
    fseek(pontArq, 0, SEEK_SET); // Posicionando pontArq no início do arquivo

    if(pontArq == NULL){
        printf("Erro com o ponteiro para o arquivo\n");
        return false;
    }

    // Escrevendo struct header no arquivo campo a campo
    fwrite(&(headerArq->status), sizeof(char), 1, pontArq);
    fwrite(&(headerArq->topo), sizeof(long int), 1, pontArq);
    fwrite(&(headerArq->proxByteOffset), sizeof(long int), 1, pontArq);
    fwrite(&(headerArq->nroRegArq), sizeof(int), 1, pontArq);
    fwrite(&(headerArq->nroRegRem), sizeof(int), 1, pontArq);

    // Escrevendo a parte semântica somente se necessário
    if(semantico){        
        // descreveIdentificador: descrição do campo idAttack
        fwrite(headerArq->descreveIdentificador, sizeof(char), 23, pontArq);

        // descreveYear: descrição do campo year
        fwrite(headerArq->descreveYear, sizeof(char), 28, pontArq);

        // descreveFinancialLoss: descrição do campo financialLoss
        fwrite(headerArq->descreveFinancialLoss, sizeof(char), 28, pontArq);

        // codDescreveCountry: código da keyword que representa o campo country
        fwrite(&(headerArq->codDescreveCountry), sizeof(char), 1, pontArq);

        // codDescreveType: código da keyword que representa o campo type
        fwrite(&(headerArq->codDescreveType), sizeof(char), 1, pontArq);

        // descreveType: descrição do campo type
        fwrite(headerArq->descreveType, sizeof(char), 38, pontArq);

        // codDescreveTargetIndustry: código da keyword que representa o campo targetIndustry
        fwrite(&(headerArq->codDescreveTargetIndustry), sizeof(char), 1, pontArq);

        // descreveTargetIndustry: descrição do campo targetIndustry
        fwrite(headerArq->descreveTargetIndustry, sizeof(char), 38, pontArq);

        // codDescreveDefense: código da keyword que representa o campo defenseMechanism
        fwrite(&(headerArq->codDescreveDefense), sizeof(char), 1, pontArq);

        // descreveDefense: descrição do campo defenseMechanism
        fwrite(headerArq->descreveDefense, sizeof(char), 67, pontArq);
    }

    return true;
}

// TODO
/*
DADO* dado_criar(char removido, int tamReg, long int prox, int idAttack, int year, float finLoss, char* country, char* attackType, char* targetInd, char* defMec){

}*/