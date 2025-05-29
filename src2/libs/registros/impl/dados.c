#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../auxiliar/auxiliar.h"
#include "../header.h"
#include "../dados.h"

/*Armazena os campos de um registro de dados*/
struct dados_
{
    char removido;       // Indica se o registro está logicamente removido.
                         // 1 = removido, 0 = não removido
    int tamanhoRegistro; // Tamanho do registro em bytes
    long int prox;       // Byteoffset do próximo registro logicamente removido.
                         // Inicializado com -1
    int idAttack;        // Código identificador do ataque
    int year;            // Ano em que o ataque ocorreu
    float financialLoss; // Prejuízo causado pelo ataque

    char country[TAM_MAX_STR];          // País onde ocorreu o ataque
    char attackType[TAM_MAX_STR];       // Tipo de ameaça à segurança cibernética
    char targetIndustry[TAM_MAX_STR];   // Setor da indústria que sofreu o ataque
    char defenseMechanism[TAM_MAX_STR]; // Estratégia de defesa usada para resolver o problema
};

/* ------------------------------------------------------------------------------------- */
/* FUNÇÕES DOS DADOS*/
/* ------------------------------------------------------------------------------------- */

// Função auxiliar
/* dado_set_tamReg():
Calcula o número de bytes do registro e atualiza na struct
Parâmetros: Ponteiro para struct
Retorna:
*/
void dado_set_tamReg(DADO *registro)
{
    if (registro == NULL)
        return;

    int contadorBytes = 20; // Inicializa o contador com o tamanho dos campos fixos

    // Adiciona ao contador o tamanho dos campos variáveis
    if (registro->country != NULL)
        contadorBytes += strlen(registro->country);
    if (registro->attackType != NULL)
        contadorBytes += strlen(registro->attackType);
    if (registro->targetIndustry != NULL)
        contadorBytes += strlen(registro->targetIndustry);
    if (registro->defenseMechanism != NULL)
        contadorBytes += strlen(registro->defenseMechanism);

    registro->tamanhoRegistro = contadorBytes;

    return;
}

DADO *dado_criar(int removido, int tamReg, long int prox, int idAttack, int year, float finLoss,
                 char *country, char *attackType, char *targetInd, char *defMec)
{
    // Alocando memória na heap para a struct
    DADO *novoRegistro = (DADO *)malloc(sizeof(DADO));
    if (novoRegistro == NULL)
        return NULL; // Erro de alocação

    /* INICIALIZANDO A STRUCT */
    // Campos de tamanho fixo
    novoRegistro->removido = removido + '0'; // Conversão int -> char
    novoRegistro->tamanhoRegistro = tamReg;
    novoRegistro->prox = prox;
    novoRegistro->idAttack = idAttack;
    novoRegistro->year = year;
    novoRegistro->financialLoss = finLoss;

    // Campos de tamanho variável, COM delimitadores
    strcpy(novoRegistro->country, formata_string_registro(country, "1"));
    strcpy(novoRegistro->attackType, formata_string_registro(attackType, "2"));
    strcpy(novoRegistro->targetIndustry, formata_string_registro(targetInd, "3"));
    strcpy(novoRegistro->defenseMechanism, formata_string_registro(defMec, "4"));

    dado_set_tamReg(novoRegistro); // Atualizando tamanho do registro

    return novoRegistro; // Retorna ponteiro para struct DADO
}

void dado_apagar(DADO **registro)
{
    if (*registro == NULL)
        return;

    // Desaloca memória da estrututa DADO e define ponteiro para NULL
    free(*registro);
    *registro = NULL;
}

char *dado_get_string(DADO *dado, int campo)
{
    if (dado == NULL)
        return NULL;

    switch (campo)
    {
    case 1:
        return dado->country;
    case 2:
        return dado->attackType;
    case 3:
        return dado->targetIndustry;
    case 4:
        return dado->defenseMechanism;
    default:
        return NULL;
    }
}

int dado_get_tamReg(DADO *dado)
{
    if (dado == NULL)
        return -1;

    return dado->tamanhoRegistro;
}

char dado_get_removido(DADO *dado)
{
    if (dado == NULL)
        return 'a';

    return dado->removido;
}

void dado_imprimir(HEADER *header, DADO *dado)
{
    if ((header == NULL) || (dado == NULL))
        return;

    // Imprimindo as descrições semânticas dos campos do dado e seus valores

    /*idAttack*/
    printf("%.*s: ", TAM_DESC_ID, header_get_descricao(header, 1));
    if (dado->idAttack == -1)
        printf("NADA CONSTA\n");
    else
        printf("%d\n", dado->idAttack);

    /*year*/
    printf("%.*s: ", TAM_DESC_YEAR, header_get_descricao(header, 2));
    if (dado->year == -1)
        printf("NADA CONSTA\n");
    else
        printf("%d\n", dado->year);

    /*country*/
    printf("%.*s: %s\n", TAM_DESC_COUNTRY, header_get_descricao(header, 4), dado->country);

    /*targetIndustry*/
    printf("%.*s: %s\n", TAM_DESC_TGT_IND, header_get_descricao(header, 6), dado->targetIndustry);

    /*attackType*/
    printf("%.*s: %s\n", TAM_DESC_TYPE, header_get_descricao(header, 5), dado->attackType);

    /*financialLoss*/
    printf("%.*s: ", TAM_DESC_FIN_LOSS, header_get_descricao(header, 3));
    if (dado->financialLoss == -1)
        printf("NADA CONSTA\n");
    else
        printf("%.2f\n", dado->financialLoss);

    /*defenseMechanism*/
    printf("%.*s: %s\n", TAM_DESC_DEF, header_get_descricao(header, 7), dado->defenseMechanism);

    return;
}

DADO *dado_ler(FILE *pontArq, DADO *dado, int byteOffset)
{
    if (pontArq == NULL)
        return NULL;

    // Criando uma nova struct do tipo dado caso uma não seja fornecida
    if (dado == NULL)
    {
        dado = (DADO *)malloc(sizeof(DADO));
        if (dado == NULL)
            return NULL;
    }

    // Posiciona na posição pós header
    fseek(pontArq, byteOffset, BYTEOFFSET_HEADER);

    // Lê o campo removida do arquivo e guarda na struct
    fread(&(dado)->removido, sizeof(char), 1, pontArq);

    // Lê os campos do arquivo e guarda na struct
    fread(&(dado)->tamanhoRegistro, sizeof(int), 1, pontArq);
    fread(&(dado)->prox, sizeof(long int), 1, pontArq);
    fread(&(dado)->idAttack, sizeof(int), 1, pontArq);
    fread(&(dado)->year, sizeof(int), 1, pontArq);
    fread(&(dado)->financialLoss, sizeof(float), 1, pontArq);

    // Cacula o tamanho dos campos da tamanho variável
    int bytesRestantes = (dado->tamanhoRegistro - 25) + 5;
    char *buffer = (char *)malloc(bytesRestantes + 1);
    fread(buffer, sizeof(char), bytesRestantes, pontArq);
    buffer[bytesRestantes] = '\0';

    // Ponteiro que aponta para o início do buffer
    char *pontCampo = buffer;
    // Lê os dados do buffer e guarda nos campos da struct
    strcpy(dado->country, separa_campo(&pontCampo, 1));
    strcpy(dado->attackType, separa_campo(&pontCampo, 2));
    strcpy(dado->targetIndustry, separa_campo(&pontCampo, 3));
    strcpy(dado->defenseMechanism, separa_campo(&pontCampo, 4));

    // Desaloca a memória do buffer e o aponta para NULL
    free(buffer);
    buffer = NULL;

    return dado;
}

void dado_escrever(FILE *pontArq, DADO *dado)
{
    // Verifica a corretude dos ponteiros
    if ((pontArq == NULL) || (dado == NULL))
        return;

    // Escreve os dados no arquivo binário
    fwrite(&(dado->removido), sizeof(char), 1, pontArq);
    fwrite(&(dado->tamanhoRegistro), sizeof(int), 1, pontArq);
    fwrite(&(dado->prox), sizeof(long int), 1, pontArq);
    fwrite(&(dado->idAttack), sizeof(int), 1, pontArq);
    fwrite(&(dado->year), sizeof(int), 1, pontArq);
    fwrite(&(dado->financialLoss), sizeof(float), 1, pontArq);

    // Escrevendo as strings caso elas não sejam nulas
    if (dado->country[0] != '$')
        fwrite(dado->country, sizeof(char), strlen(dado->country), pontArq);
    if (dado->attackType[0] != '$')
        fwrite(dado->attackType, sizeof(char), strlen(dado->attackType), pontArq);
    if (dado->targetIndustry[0] != '$')
        fwrite(dado->targetIndustry, sizeof(char), strlen(dado->targetIndustry), pontArq);
    if (dado->defenseMechanism[0] != '$')
        fwrite(dado->defenseMechanism, sizeof(char), strlen(dado->defenseMechanism), pontArq);

    return;
}