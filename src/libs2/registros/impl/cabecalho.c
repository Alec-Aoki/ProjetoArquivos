#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../auxiliar.h"
#include "cabecalho.h"

/*Armazena os campos de um registro de header*/
struct header_
{
    char status;             // Consistência do arquivo de dados. 0 = inconsistente, 1 = consistente
    long int topo;           // Byteoffset do primeiro registro logicamente removido. -1 = nenhum registro removido
    long int proxByteOffset; // Valor do próximo byteoffset disponível
    int nroRegArq;           // Quantidade de registros não removidos
    int nroRegRem;           // Quantidade de registros removidos

    char descreveIdentificador[TAM_DESC_ID];       // Descrição do campo idAttack
    char descreveYear[TAM_DESC_YEAR];              // Descrição do campo year
    char descreveFinancialLoss[TAM_DESC_FIN_LOSS]; // Descrição do campo financialLoss
    char codDescreveCountry;                       // Código da keyword que representa o campo country
    char descreveCountry[TAM_DESC_COUNTRY];        // Descrição do campo country
    char codDescreveType;                          // Código da keyword que representa o campo attackType
    char descreveType[TAM_DESC_TYPE];              // Descrição do campo attackType
    char codDescreveTargetIndustry;                // Código da keyword que representa o campo targetIndustry
    char descreveTargetIndustry[TAM_DESC_TGT_IND]; // Descrição do campo targetIndustry
    char codDescreveDefense;                       // Código da keyword que representa o campo defenseMechanism
    char descreveDefense[TAM_DESC_DEF];            // Descrição do campo defenseMechanism
};

/* ------------------------------------------------------------------------------------- */
/* FUNÇÕES DO HEADER*/
/* ------------------------------------------------------------------------------------- */

HEADER *header_criar(char *descIdent, char *descYear, char *descFinLoss, char *descCountry, char *descType, char *descTargInd, char *descDef)
{
    HEADER *novoHeader = (HEADER *)malloc(sizeof(HEADER)); // Alocando dinâmicamente uma struct do tipo HEADER
    if (novoHeader == NULL)
        return NULL; // Erro de alocação de memória

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

void header_apagar(HEADER **header)
{
    if (*header == NULL)
        return;

    free(*header);  // Desalocando memória
    *header = NULL; // Definindo o ponteiro para NULL

    return;
}

void header_set_status(HEADER *header, char status)
{
    if (header == NULL)
        return;

    header->status = status; // Definindo o novo status
    return;
}

void header_set_proxByteOffset(HEADER *header, long int proxByOff)
{
    if (header == NULL)
        return;

    header->proxByteOffset = proxByOff;
    return;
}

void header_set_nroRegArq(HEADER *header, int nroRegAq)
{
    if (header == NULL)
        return;

    header->nroRegArq = nroRegAq;
    return;
}

int header_get_nroRegArq(HEADER *header)
{
    if (header == NULL)
        return -1;

    return header->nroRegArq;
}