#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "auxiliar.h"
#include "registros.h"

/*Armazena os campos de um registro de header*/
struct header_{
    char status; // Consistência do arquivo de dados. 0 = inconsistente, 1 = consistente
    long int topo; // Byteoffset do primeiro registro logicamente removido. -1 = nenhum registro removido
    long int proxByteOffset; // Valor do próximo byteoffset disponível
    int nroRegArq; // Quantidade de registros não removidos
    int nroRegRem; // Quantidade de registros removidos

    char descreveIdentificador[TAM_DESC_ID]; // Descrição do campo idAttack
    char descreveYear[TAM_DESC_YEAR]; // Descrição do campo year
    char descreveFinancialLoss[TAM_DESC_FIN_LOSS]; // Descrição do campo financialLoss
    char codDescreveCountry; // Código da keyword que representa o campo country
    char descreveCountry[TAM_DESC_COUNTRY]; // Descrição do campo country
    char codDescreveType; // Código da keyword que representa o campo attackType
    char descreveType[TAM_DESC_TYPE]; // Descrição do campo attackType
    char codDescreveTargetIndustry; // Código da keyword que representa o campo targetIndustry
    char descreveTargetIndustry[TAM_DESC_TGT_IND]; // Descrição do campo targetIndustry
    char codDescreveDefense; // Código da keyword que representa o campo defenseMechanism
    char descreveDefense[TAM_DESC_DEF]; // Descrição do campo defenseMechanism
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

/* ------------------------------------------------------------------------------------- */
/* FUNÇÕES DO HEADER*/
/* ------------------------------------------------------------------------------------- */

HEADER* header_criar(char* descIdent, char* descYear, char* descFinLoss, char* descCountry, char* descType, char* descTargInd, char* descDef){
    HEADER* novoHeader = (HEADER *) malloc(sizeof(HEADER)); // Alocando dinâmicamente uma struct do tipo HEADER
    if(novoHeader == NULL) return NULL; // Erro de alocação de memória

    /* INICIALIZANDO O HEADER */
    // Campos de valor variável
    novoHeader->status = '0';
    novoHeader->topo = -1;
    novoHeader->proxByteOffset = 0;
    novoHeader->nroRegArq = 0;
    novoHeader->nroRegRem = 0;

    // Campos de valor fixo (semânticos)
    novoHeader->codDescreveCountry = '1';
    novoHeader->codDescreveType = '2';
    novoHeader->codDescreveTargetIndustry = '3';
    novoHeader->codDescreveDefense = '4';

    // Strings
    strncpy(novoHeader->descreveIdentificador, descIdent, TAM_DESC_ID);
    strncpy(novoHeader->descreveYear, descYear, TAM_DESC_YEAR);
    strncpy(novoHeader->descreveFinancialLoss, descFinLoss, TAM_DESC_FIN_LOSS);
    strncpy(novoHeader->descreveCountry, descCountry, TAM_DESC_COUNTRY);
    strncpy(novoHeader->descreveType, descType, TAM_DESC_TYPE);
    strncpy(novoHeader->descreveTargetIndustry, descTargInd, TAM_DESC_TGT_IND);
    strncpy(novoHeader->descreveDefense, descDef, TAM_DESC_DEF);

    return novoHeader; // Retornando ponteiro para HEADER
}

void header_apagar(HEADER** header){
    if(*header == NULL) return;

    free(*header); // Desalocando memória
    *header = NULL; // Definindo o ponteiro para NULL

    return;
}

void header_escrever(FILE* pontArq, HEADER* headerArq, bool semantico){
    if((pontArq == NULL) || (headerArq == NULL)) return;

    fseek(pontArq, 0, SEEK_SET); // Posicionando pontArq no início do arquivo

    // Escrevendo os campos variáveis da struct header no arquivo
    fwrite(&(headerArq->status), sizeof(char), 1, pontArq);
    fwrite(&(headerArq->topo), sizeof(long int), 1, pontArq);
    fwrite(&(headerArq->proxByteOffset), sizeof(long int), 1, pontArq);
    fwrite(&(headerArq->nroRegArq), sizeof(int), 1, pontArq);
    fwrite(&(headerArq->nroRegRem), sizeof(int), 1, pontArq);

    // Escrevendo a parte semântica somente se necessário
    if(semantico){        
        // descreveIdentificador: descrição do campo idAttack
        fwrite(headerArq->descreveIdentificador, sizeof(char), TAM_DESC_ID, pontArq);

        // descreveYear: descrição do campo year
        fwrite(headerArq->descreveYear, sizeof(char), TAM_DESC_YEAR, pontArq);

        // descreveFinancialLoss: descrição do campo financialLoss
        fwrite(headerArq->descreveFinancialLoss, sizeof(char), TAM_DESC_FIN_LOSS, pontArq);

        // codDescreveCountry: código da keyword que representa o campo country
        fwrite(&(headerArq->codDescreveCountry), sizeof(char), 1, pontArq);

        // descreveYear: descrição do campo country
        fwrite(headerArq->descreveCountry, sizeof(char), TAM_DESC_COUNTRY, pontArq);

        // codDescreveType: código da keyword que representa o campo type
        fwrite(&(headerArq->codDescreveType), sizeof(char), 1, pontArq);

        // descreveType: descrição do campo type
        fwrite(headerArq->descreveType, sizeof(char), TAM_DESC_TYPE, pontArq);

        // codDescreveTargetIndustry: código da keyword que representa o campo targetIndustry
        fwrite(&(headerArq->codDescreveTargetIndustry), sizeof(char), 1, pontArq);

        // descreveTargetIndustry: descrição do campo targetIndustry
        fwrite(headerArq->descreveTargetIndustry, sizeof(char), TAM_DESC_TGT_IND, pontArq);

        // codDescreveDefense: código da keyword que representa o campo defenseMechanism
        fwrite(&(headerArq->codDescreveDefense), sizeof(char), 1, pontArq);

        // descreveDefense: descrição do campo defenseMechanism
        fwrite(headerArq->descreveDefense, sizeof(char), TAM_DESC_DEF, pontArq);
    }

    return;
}

HEADER* header_ler(FILE* pontArq, HEADER* header){
    if(pontArq == NULL) return NULL; // Erro

    // Criando uma nova struct do tipo header caso uma não seja fornecida
    if(header == NULL){;
        header = (HEADER *) malloc(sizeof(HEADER));
        if (header == NULL) return NULL;
    }

    fseek(pontArq, 0, SEEK_SET); // Posicionando ponteiro no início do arquivo

    // Lendo campos que não são strings
    fread(&(header->status), sizeof(char), 1, pontArq);
    fread(&(header->topo), sizeof(long int), 1, pontArq);
    fread(&(header->proxByteOffset), sizeof(long int), 1, pontArq);
    fread(&(header->nroRegArq), sizeof(int), 1, pontArq);
    fread(&(header->nroRegRem), sizeof(int), 1, pontArq);

    // Lendo strings
    fread(header->descreveIdentificador, sizeof(char), TAM_DESC_ID, pontArq);
    fread(header->descreveYear, sizeof(char), TAM_DESC_YEAR, pontArq);
    fread(header->descreveFinancialLoss, sizeof(char), TAM_DESC_FIN_LOSS, pontArq);
    fread(&(header->codDescreveCountry), sizeof(char), 1, pontArq);
    fread(header->descreveCountry, sizeof(char), TAM_DESC_COUNTRY, pontArq);
    fread(&(header->codDescreveType), sizeof(char), 1, pontArq);
    fread(header->descreveType, sizeof(char), TAM_DESC_TYPE, pontArq);
    fread(&(header->codDescreveTargetIndustry), sizeof(char), 1, pontArq);
    fread(header->descreveTargetIndustry, sizeof(char), TAM_DESC_TGT_IND, pontArq);
    fread(&(header->codDescreveDefense), sizeof(char), 1, pontArq);
    fread(header->descreveDefense, sizeof(char), TAM_DESC_DEF, pontArq);

    return header;
}

void header_set_status(HEADER* header, char status){
    if(header == NULL) return;

    header->status = status; // Definindo o novo status
    return;
}

void header_set_proxByteOffset(HEADER* header, long int proxByOff){
    if(header == NULL) return;

    header->proxByteOffset = proxByOff;
    return;
}

void header_set_nroRegArq(HEADER* header, int nroRegAq){
    if(header == NULL) return;

    header->nroRegArq = nroRegAq;
    return;
}

int header_get_nroRegArq(HEADER* header){
    if(header == NULL) return -1;

    return header->nroRegArq;
}

/* ------------------------------------------------------------------------------------- */
/* FUNÇÕES DOS DADOS */
/* ------------------------------------------------------------------------------------- */

// Função auxiliar, explicada mais adiante
void dado_set_tamReg (DADO *registro);

DADO* dado_criar(char removido, int tamReg, long int prox, int idAttack, int year, float finLoss, char* country, char* attackType, char* targetInd, char* defMec){
    // Alocando memória na heap para a struct
    DADO *novoRegistro = (DADO *) malloc(sizeof(DADO));
    if (novoRegistro == NULL) return NULL; // Erro de alocação

    /* INICIALIZANDO A STRUCT */
    // Campos de tamanho fixo
    novoRegistro->removido = removido;
    novoRegistro->tamanhoRegistro = tamReg;
    novoRegistro->prox = prox;
    novoRegistro->idAttack = idAttack;
    novoRegistro->year = year;
    novoRegistro->financialLoss = finLoss;


    // Campos de tamanho variável, COM delimitadores
    novoRegistro->country = formata_string_registro(country, "1");
    novoRegistro->attackType = formata_string_registro(attackType, "2");
    novoRegistro->targetIndustry = formata_string_registro(targetInd, "3");
    novoRegistro->defenseMechanism = formata_string_registro(defMec, "4");

    dado_set_tamReg(novoRegistro); // Atualizando tamanho do registro

    return novoRegistro; // Retorna ponteiro para struct DADO
}

void dado_apagar(DADO **registro){
    if(*registro == NULL) return;

    // Desaloca a memória das strings de tamanhos variáveis
    if ((*registro)->country != NULL) free((*registro)->country);
    if ((*registro)->attackType != NULL) free((*registro)->attackType);
    if ((*registro)->targetIndustry != NULL) free((*registro)->targetIndustry);
    if ((*registro)->defenseMechanism != NULL) free((*registro)->defenseMechanism);    

    // Desaloca memória da estrututa DADO e define ponteiro para NULL
    free(*registro);
    *registro = NULL; 
}

void dado_escrever (FILE *pontArq, DADO *dado){
    // Verifica a corretude dos ponteiros
    if ((pontArq == NULL) || (dado == NULL)) return;
        
    // Escreve os dados no arquivo binário
    fwrite(&(dado->removido), sizeof(char), 1, pontArq);
    fwrite(&(dado->tamanhoRegistro), sizeof(int), 1, pontArq);
    fwrite(&(dado->prox), sizeof(long int), 1, pontArq);
    fwrite(&(dado->idAttack), sizeof(int), 1, pontArq);
    fwrite(&(dado->year), sizeof(int), 1, pontArq);
    fwrite(&(dado->financialLoss), sizeof(float), 1, pontArq);

    // Escrevendo as strings caso elas não sejam nulas
    if(dado->country != NULL) fwrite(dado->country, sizeof(char), strlen(dado->country), pontArq);
    if(dado->attackType != NULL) fwrite(dado->attackType, sizeof(char), strlen(dado->attackType), pontArq);
    if(dado->targetIndustry != NULL) fwrite(dado->targetIndustry, sizeof(char), strlen(dado->targetIndustry), pontArq);
    if(dado->defenseMechanism != NULL) fwrite(dado->defenseMechanism, sizeof(char), strlen(dado->defenseMechanism), pontArq);
    
    return;
}

DADO* dado_ler(FILE* pontArq, DADO* dado, int byteOffset){
    if(pontArq == NULL) return NULL;

    // Criando uma nova struct do tipo dado caso uma não seja fornecida
    if(dado == NULL){
        dado = (DADO *) malloc(sizeof(DADO));
        if (dado == NULL) return NULL;
    }

    // Posiciona na posição pós header
    fseek(pontArq, byteOffset, SEEK_SET);
    
    // Lê o campo removida do arquivo e guarda na struct
    fread(&(dado)->removido, sizeof(char), 1, pontArq);
    // Verifica se o dado está logicamente removido
    if (dado->removido == 1) return NULL;

    // Lê os campos do arquivo e guarda na struct
    fread(&(dado)->tamanhoRegistro, sizeof(int), 1, pontArq);
    fread(&(dado)->prox, sizeof(long int), 1, pontArq);
    fread(&(dado)->idAttack, sizeof(int), 1, pontArq);
    fread(&(dado)->year, sizeof(int), 1, pontArq);
    fread(&(dado)->financialLoss, sizeof(float), 1, pontArq);

    // Cacula o tamanho dos campos da tamanho variável
    int bytesRestantes = dado->tamanhoRegistro - 25;
    char *buffer = (char *) malloc(bytesRestantes + 1);
    fread(buffer, sizeof(char), bytesRestantes, pontArq);
    buffer[bytesRestantes] = '\0';
    
    // Ponteiro que aponta para o início do buffer
    char *pontCampo = buffer;

    // Lê os dados do buffer e guarda nos campos da struct
    dado->country = separa_campo(&pontCampo);
    dado->attackType = separa_campo(&pontCampo);
    dado->targetIndustry = separa_campo(&pontCampo);
    dado->defenseMechanism = separa_campo(&pontCampo);

    // Desaloca a memória do buffer e o aponta para NULL
    free(buffer);
    buffer = NULL;

    return dado;
}

void dado_imprimir(HEADER* header, DADO* dado){
    if((header == NULL) || (dado == NULL)) return;

    // Imprimindo as descrições semânticas dos campos do dado e seus valores
    printf("%.*s : %d\n", TAM_DESC_ID, header->descreveIdentificador, dado->idAttack);
    printf("%.*s : %d\n", TAM_DESC_YEAR, header->descreveYear, dado->year);
    printf("%.*s : %s\n", TAM_DESC_COUNTRY, header->descreveCountry, dado->country);
    printf("%.*s : %s\n", TAM_DESC_TGT_IND, header->descreveTargetIndustry, dado->targetIndustry);
    printf("%.*s : %s\n", TAM_DESC_TYPE, header->descreveType, dado->attackType);
    printf("%.*s : %.2f\n", TAM_DESC_FIN_LOSS, header->descreveFinancialLoss, dado->financialLoss);
    printf("%.*s : %s\n", TAM_DESC_DEF, header->descreveDefense, dado->defenseMechanism);
    
    return;
}

/* dado_set_tamReg():
Calcula o número de bytes do registro e atualiza na struct
Parâmetros: Ponteiro para struct
Retorna: 
*/
void dado_set_tamReg (DADO *registro){
    if(registro == NULL) return;

    int contadorBytes = 25; // Inicializa o contador com o tamanho dos campos fixos
        
    // Adiciona ao contador o tamanho dos campos variáveis
    if (registro->country != NULL) contadorBytes += strlen(registro->country);
    if (registro->attackType != NULL) contadorBytes += strlen(registro->attackType);
    if (registro->targetIndustry != NULL) contadorBytes += strlen(registro->targetIndustry);
    if (registro->defenseMechanism != NULL) contadorBytes += strlen(registro->defenseMechanism);
    
    registro->tamanhoRegistro = contadorBytes;

    return;  
}

int dado_get_tamanho(DADO* dado){
    if(dado == NULL) return -1;

    return dado->tamanhoRegistro;
}

int dado_get_idAttacK(DADO* dado){
    if(dado == NULL) return -1;

    return dado->idAttack;
}

int dado_get_year(DADO* dado){
    if(dado == NULL) return -1;

    return dado->year;
}

float dado_get_finLoss(DADO* dado){
    if(dado == NULL) return -1;

    return dado->financialLoss;
}

char* dado_get_country(DADO* dado){
    if(dado == NULL) return NULL;

    return dado->country;
}

char* dado_get_attackType(DADO* dado){
    if(dado == NULL) return NULL;

    return dado->attackType;
}

char* dado_get_targetIndustry(DADO* dado){
    if(dado == NULL) return NULL;

    return dado->targetIndustry;
}

char* dado_get_defenseMech(DADO* dado){
    if(dado == NULL) return NULL;

    return dado->defenseMechanism;
}