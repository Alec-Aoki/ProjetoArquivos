#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../auxiliar/auxiliar.h"
#include "../header.h"

/*Armazena os campos de um registro de header*/
struct header_
{
    char status;
    long int topo;
    long int proxByteOffset;
    int nroRegArq;
    int nroRegRem;

    char descreveIdentificador[TAM_DESC_ID];
    char descreveYear[TAM_DESC_YEAR];
    char descreveFinancialLoss[TAM_DESC_FIN_LOSS];
    char codDescreveCountry;
    char descreveCountry[TAM_DESC_COUNTRY];
    char codDescreveType;
    char descreveType[TAM_DESC_TYPE];
    char codDescreveTargetIndustry;
    char descreveTargetIndustry[TAM_DESC_TGT_IND];
    char codDescreveDefense;
    char descreveDefense[TAM_DESC_DEF];
};

/* header_criar():
Aloca memória para uma struct do tipo HEADER e a inicializa
Retorna: ponteiro para a struct do tipo header
*/
HEADER *header_criar()
{
    HEADER *novoHeader = (HEADER *)malloc(sizeof(HEADER)); // Alocando dinâmicamente uma struct do tipo HEADER
    if (novoHeader == NULL)
        return NULL; // Erro de alocação de memória

    /* INICIALIZANDO O HEADER */
    // Campos de valor variável
    novoHeader->status = '1';
    novoHeader->topo = -1;
    novoHeader->proxByteOffset = BYTEOFFSET_HEADER;
    novoHeader->nroRegArq = 0;
    novoHeader->nroRegRem = 0;

    // Campos de valor fixo (semânticos)
    novoHeader->codDescreveCountry = '1';
    novoHeader->codDescreveType = '2';
    novoHeader->codDescreveTargetIndustry = '3';
    novoHeader->codDescreveDefense = '4';

    // Strings
    strcpy(novoHeader->descreveIdentificador, "NADA CONSTA");
    strcpy(novoHeader->descreveYear, "NADA CONSTA");
    strcpy(novoHeader->descreveFinancialLoss, "NADA CONSTA");
    strcpy(novoHeader->descreveCountry, "NADA CONSTA");
    strcpy(novoHeader->descreveType, "NADA CONSTA");
    strcpy(novoHeader->descreveTargetIndustry, "NADA CONSTA");
    strcpy(novoHeader->descreveDefense, "NADA CONSTA");

    return novoHeader; // Retornando ponteiro para HEADER
}

/* header_set():
Define os campos de uma struct do tipo header. Se uma struct não for fornecida, cria uma.
Parâmetros: valores para os campos do header
Retorna: ponteiro para a struct do tipo header
*/
HEADER *header_set(HEADER *header, int status, long int topo,
                   long int proxByteOffset, int nroRegArq,
                   int nroRegRem, char *descIdent,
                   char *descYear, char *descFinLoss,
                   char *descCountry, char *descType,
                   char *descTargInd, char *descDef)
{
    if (header == NULL)
        header = header_criar();

    /* INICIALIZANDO O HEADER */
    // Campos de valor variável
    if (status != -2)
        header->status = status + '0'; // Conversão int -> char
    if (topo != -2)
        header->topo = topo;
    if (proxByteOffset != -2)
        header->proxByteOffset = proxByteOffset;
    if (nroRegArq != -2)
        header->nroRegArq = nroRegArq;
    if (nroRegRem != -2)
        header->nroRegRem = nroRegRem;

    // Strings
    if (descIdent != NULL)
        strncpy(header->descreveIdentificador, descIdent, TAM_DESC_ID);
    if (descYear != NULL)
        strncpy(header->descreveYear, descYear, TAM_DESC_YEAR);
    if (descFinLoss != NULL)
        strncpy(header->descreveFinancialLoss, descFinLoss, TAM_DESC_FIN_LOSS);
    if (descCountry != NULL)
        strncpy(header->descreveCountry, descCountry, TAM_DESC_COUNTRY);
    if (descType != NULL)
        strncpy(header->descreveType, descType, TAM_DESC_TYPE);
    if (descTargInd != NULL)
        strncpy(header->descreveTargetIndustry, descTargInd, TAM_DESC_TGT_IND);
    if (descDef != NULL)
        strncpy(header->descreveDefense, descDef, TAM_DESC_DEF);

    return header; // Retornando ponteiro para HEADER
}

/* header_apagar():
Desaloca uma struct do tipo header e define seu ponteiro para NULL
Parâmetros: ponteiro de ponteiro para a struct a ser desalocada
*/
void header_apagar(HEADER **header)
{
    if (*header == NULL)
        return;

    free(*header);  // Desalocando memória
    *header = NULL; // Definindo o ponteiro para NULL

    return;
}

/* header_get_int():
Retorna o valor de um campo int
Parâmetros: ponteiro pra struct do tipo header, inteiro de 1 a 2 (campo)
    1: nroRegArq
    2: nroRegRem
Retorna: valor do campo (-1 se não encontrado ou header nulo)
*/
int header_get_int(HEADER *header, int campo)
{
    if (header == NULL)
        return -1;

    switch (campo)
    {
    case 1:
        return header->nroRegArq;
    case 2:
        return header->nroRegRem;
    default:
        break;
    }

    return -1;
}

/* header_get_longint():
Retorna o valor de um campo long int
Parâmetros: ponteiro pra struct do tipo header, inteiro de 1 a 2 (campo)
    1: topo
    2: proxByteOffset
Retorna: valor do campo (-1 se não encontrado ou header nulo)
*/
long int header_get_longint(HEADER *header, int campo)
{
    if (header == NULL)
        return -1;

    switch (campo)
    {
    case 1:
        return header->topo;
    case 2:
        return header->proxByteOffset;
    default:
        break;
    }

    return -1;
}

/* header_get_descricao():
Retorna a string "descreve" de um campo
Parâmetros: ponteiro pra struct do tipo header, inteiro de 1 a 7 (campo)
    1: descreveIdentificador
    2: descreveYear
    3: descreveFinancialLoss
    4: descrevCountry
    5: descreveType
    6: descreveTargetIndustry
    7: descreveDefense
Retorna: ponteiro para string (NULL se não encontrado ou header nulo)
*/
char *header_get_descricao(HEADER *header, int campo)
{
    if (header == NULL)
        return NULL;

    switch (campo)
    {
    case 1:
        return header->descreveIdentificador;
    case 2:
        return header->descreveYear;
    case 3:
        return header->descreveFinancialLoss;
    case 4:
        return header->descreveCountry;
    case 5:
        return header->descreveType;
    case 6:
        return header->descreveTargetIndustry;
    case 7:
        return header->descreveDefense;
    default:
        return NULL;
    }
}

/* header_ler():
Lê os campos do header de um arquivo binário e guarda em uma struct do tipo HEADER
Parâmetros: ponteiro para arquivo, ponteiro para header
Retorna: ponteiro para header
*/
HEADER *header_ler(FILE *pontArq, HEADER *header)
{
    if (pontArq == NULL)
        return NULL; // Erro

    // Criando uma nova struct do tipo header caso uma não seja fornecida
    if (header == NULL)
    {
        header = (HEADER *)malloc(sizeof(HEADER));
        if (header == NULL)
            return NULL;
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

/* header_escrever():
Escreve um header passado no arquivo binário
Parâmetros: ponteiro para um arquivo, ponteiro para um header e valor booleano
(true = escrever string semanticas, false = escrever somente struct)
*/
void header_escrever(FILE *pontArq, HEADER *headerArq, bool semantico)
{
    if ((pontArq == NULL) || (headerArq == NULL))
        return;

    // Escrevendo os campos variáveis da struct header no arquivo
    fwrite(&(headerArq->status), sizeof(char), 1, pontArq);
    fwrite(&(headerArq->topo), sizeof(long int), 1, pontArq);
    fwrite(&(headerArq->proxByteOffset), sizeof(long int), 1, pontArq);
    fwrite(&(headerArq->nroRegArq), sizeof(int), 1, pontArq);
    fwrite(&(headerArq->nroRegRem), sizeof(int), 1, pontArq);

    // Escrevendo a parte semântica somente se necessário
    if (semantico)
    {
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