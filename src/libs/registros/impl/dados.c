#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../auxiliar/auxiliar.h"
#include "../header.h"
#include "../dados.h"

/*Armazena os campos de um registro de dados*/
struct dados_
{
    char removido;
    int tamanhoRegistro;
    long int prox;
    int idAttack;
    int year;
    float financialLoss;

    char country[TAM_MAX_STR];
    char attackType[TAM_MAX_STR];
    char targetIndustry[TAM_MAX_STR];
    char defenseMechanism[TAM_MAX_STR];
};

/* dado_atualizar_tamReg():
Calcula o número de bytes do registro e atualiza na struct
Parâmetros: Ponteiro para struct
*/
void dado_atualizar_tamReg(DADO *registro)
{
    if (registro == NULL)
        return;

    int contadorBytes = 20; // Inicializa o contador com o tamanho dos campos fixos

    // Adiciona ao contador o tamanho dos campos variáveis
    if (strcmp(registro->country, "NADA CONSTA") != 0)
        contadorBytes += strlen(registro->country);
    if (strcmp(registro->attackType, "NADA CONSTA") != 0)
        contadorBytes += strlen(registro->attackType);
    if (strcmp(registro->targetIndustry, "NADA CONSTA") != 0)
        contadorBytes += strlen(registro->targetIndustry);
    if (strcmp(registro->defenseMechanism, "NADA CONSTA") != 0)
        contadorBytes += strlen(registro->defenseMechanism);

    registro->tamanhoRegistro = contadorBytes;

    return;
}

/* dado_criar():
Aloca memória para uma struct do tipo dado e inicializa seus campos
Retorna: ponteiro para dado
*/
DADO *dado_criar()
{
    // Alocando memória na heap para a struct
    DADO *novoRegistro = (DADO *)malloc(sizeof(DADO));
    if (novoRegistro == NULL)
        return NULL; // Erro de alocação

    /* INICIALIZANDO A STRUCT */
    // Campos de tamanho fixo
    novoRegistro->removido = 0 + '0';
    novoRegistro->tamanhoRegistro = -1;
    novoRegistro->prox = -1;
    novoRegistro->idAttack = -1;
    novoRegistro->year = -1;
    novoRegistro->financialLoss = -1;

    // Campos de tamanho variável, COM delimitadores
    strcpy(novoRegistro->country, "NADA CONSTA");
    strcpy(novoRegistro->attackType, "NADA CONSTA");
    strcpy(novoRegistro->targetIndustry, "NADA CONSTA");
    strcpy(novoRegistro->defenseMechanism, "NADA CONSTA");

    dado_atualizar_tamReg(novoRegistro); // Atualizando tamanho do registro

    return novoRegistro; // Retorna ponteiro para struct DADO
}

/* dado_set():
Define campos de uma struct dado. Caso uma struct não seja fornecida, cria uma
Caso -2 ou NULL seja fornecido, não altera o campo da struct.
Parâmetros: valores dos campos da struct
Retorna: ponteiro para dado
*/
DADO *dado_set(DADO *dado, int removido, int tamReg, long int prox, int idAttack, int year, float finLoss,
               char *country, char *attackType, char *targetInd, char *defMec)
{
    // Alocando memória na heap para a struct
    bool dadoNovo = false;
    if (dado == NULL)
    {
        dadoNovo = true;
        dado = dado_criar();
    }

    /* INICIALIZANDO A STRUCT */
    // Campos de tamanho fixo
    if (removido != -2)
        dado->removido = removido + '0'; // Conversão int -> char
    if (tamReg != -2)
        dado->tamanhoRegistro = tamReg;
    if (prox != -2)
        dado->prox = prox;
    if (idAttack != -2)
        dado->idAttack = idAttack;
    if (year != -2)
        dado->year = year;
    if (finLoss != -2)
        dado->financialLoss = finLoss;

    // Campos de tamanho variável, COM delimitadores
    if (country != NULL && strcmp(country, "$") != 0)
    {
        if (strcmp(country, "NULO") == 0)
            strcpy(dado->country, "NADA CONSTA");
        else
            strcpy(dado->country, formata_string_registro(country, "1"));
    }
    if (attackType != NULL && strcmp(attackType, "$") != 0)
    {
        if (strcmp(attackType, "NULO") == 0)
            strcpy(dado->attackType, "NADA CONSTA");
        else
            strcpy(dado->attackType, formata_string_registro(attackType, "2"));
    }
    if (targetInd != NULL && strcmp(targetInd, "$") != 0)
    {
        if (strcmp(targetInd, "NULO") == 0)
            strcpy(dado->targetIndustry, "NADA CONSTA");
        else
            strcpy(dado->targetIndustry, formata_string_registro(targetInd, "3"));
    }
    if (defMec != NULL && strcmp(defMec, "$") != 0)
    {
        if (strcmp(defMec, "NULO") == 0)
            strcpy(dado->defenseMechanism, "NADA CONSTA");
        else
            strcpy(dado->defenseMechanism, formata_string_registro(defMec, "4"));
    }

    // Atualizando tamanho do registro somente se ele acabou de ser criado
    if (dadoNovo)
        dado_atualizar_tamReg(dado);

    return dado; // Retorna ponteiro para struct DADO
}

/* dado_apagar():
Desaloca memória da struct e dos campos de tamanho variável
Parâmetro: ponteiro para ponteiro da struct
*/
void dado_apagar(DADO **registro)
{
    if (*registro == NULL)
        return;

    // Desaloca memória da estrututa DADO e define ponteiro para NULL
    free(*registro);
    *registro = NULL;
}

/* dado_get_prox():
Retorna o valor do campo prox de um dado
Parâmetros: ponteiro pra struct do tipo dado
Retorna: valor do campo (-1 se dado nulo)
*/
long int dado_get_prox(DADO *dado)
{
    if (dado == NULL)
        return -1;

    return dado->prox;
}

/* dado_get_string():
Retorna a string de um campo
Parâmetros: ponteiro pra struct do tipo dado, inteiro de 1 a 4 (campo)
    1: country
    2: attackType
    3: targetIndustry
    4: defenseMechanism
Retorna: ponteiro para string (NULL se não encontrado ou dado nulo)
*/
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

/* dado_get_int():
Retorna o valor de um campo do tipo int
Parâmetros: ponteiro pra struct do tipo dado, inteiro de 1 a 3 (campo)
    1: idAttack
    2: year
    3: tamanhoRegistro
Retorna: valor do campo (-1 se mal sucedido)
*/
int dado_get_int(DADO *dado, int campo)
{
    if (dado == NULL)
        return -1;

    switch (campo)
    {
    case 1:
        return dado->idAttack;
    case 2:
        return dado->year;
    case 3:
        return dado->tamanhoRegistro;
    default:
        return -1;
    }
}

/* dado_get_finLoss():
Retorna o valor do campo financialLoss
Parâmetros: ponteiro pra struct do tipo dado
Retorna: valor do campo (-1 se mal sucedido)
*/
float dado_get_finLoss(DADO *dado)
{
    if (dado == NULL)
        return -1;

    return dado->financialLoss;
}

/* dado_get_removido():
Retorna o valor do campo removido de um dado
Parâmetros: ponteiro pra struct do tipo dado
Retorna: valor do campo ('a' se dado nulo)
*/
char dado_get_removido(DADO *dado)
{
    if (dado == NULL)
        return 'a'; // Erro

    return dado->removido;
}

/* dado_imprimir():
Imprime um dado usando as descrições semânticas do header
Parâmetros: ponteiro para o header, ponteiro para o dado
*/
void dado_imprimir(HEADER *header, DADO *dado)
{
    if ((header == NULL) || (dado == NULL))
        return;

    // Imprimindo as descrições semânticas dos campos do dado e seus valores,
    // SEM DELIMITADORES
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
    printf("%.*s: ", TAM_DESC_COUNTRY, header_get_descricao(header, 4));
    if (strcmp(dado->country, "NADA CONSTA") == 0)
        printf("NADA CONSTA\n");
    else
        printf("%.*s\n", (int)strlen(dado->country) - 2, dado->country + 1);

    /*targetIndustry*/
    printf("%.*s: ", TAM_DESC_TGT_IND, header_get_descricao(header, 6));
    if (strcmp(dado->targetIndustry, "NADA CONSTA") == 0)
        printf("NADA CONSTA\n");
    else
        printf("%.*s\n", (int)strlen(dado->targetIndustry) - 2, dado->targetIndustry + 1);

    /*attackType*/
    printf("%.*s: ", TAM_DESC_TYPE, header_get_descricao(header, 5));
    if (strcmp(dado->attackType, "NADA CONSTA") == 0)
        printf("NADA CONSTA\n");
    else
        printf("%.*s\n", (int)strlen(dado->attackType) - 2, dado->attackType + 1);

    /*financialLoss*/
    printf("%.*s: ", TAM_DESC_FIN_LOSS, header_get_descricao(header, 3));
    if (dado->financialLoss == -1)
        printf("NADA CONSTA\n");
    else
        printf("%.2f\n", dado->financialLoss);

    /*defenseMechanism*/
    printf("%.*s: ", TAM_DESC_DEF, header_get_descricao(header, 7));
    if (strcmp(dado->defenseMechanism, "NADA CONSTA") == 0)
        printf("NADA CONSTA\n");
    else
        printf("%.*s\n", (int)strlen(dado->defenseMechanism) - 2, dado->defenseMechanism + 1);

    return;
}

/* dado_ler():
Lê um registro do arquivo e guarda numa struct DADO
Parâmetros: ponteiro para arquivo, ponteiro para struct DADO, byteOffset do registro
Retorna: ponteiro para dado
*/
DADO *dado_ler(FILE *pontArq, DADO *dado, int byteOffset)
{
    if (pontArq == NULL)
        return NULL;

    // Criando uma nova struct do tipo dado caso uma não seja fornecida
    if (dado == NULL)
        dado = dado_criar();

    // Posiciona na posição pós header
    fseek(pontArq, byteOffset, SEEK_SET);

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
    // Lê os dados do buffer e guarda nos campos da struct (COM DELIMITADORES)
    strcpy(dado->country, formata_string_registro(separa_campo(&pontCampo, 1), "1"));
    strcpy(dado->attackType, formata_string_registro(separa_campo(&pontCampo, 2), "2"));
    strcpy(dado->targetIndustry, formata_string_registro(separa_campo(&pontCampo, 3), "3"));
    strcpy(dado->defenseMechanism, formata_string_registro(separa_campo(&pontCampo, 4), "4"));

    // Desaloca a memória do buffer e o aponta para NULL
    free(buffer);
    buffer = NULL;

    return dado;
}

/*dado_escrever():
Escreve os campos de uma struct dado em um arquivo.
Caso lixo > 0, escreve aquela quantidade de lixo em seguida
Parâmetros: ponteiro para arquivo, ponteiro para uma struct dado, inteiro
*/
void dado_escrever(FILE *pontArq, DADO *dado, int lixo)
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
    if (strcmp(dado->country, "NADA CONSTA") != 0)
        fwrite(dado->country, sizeof(char), strlen(dado->country), pontArq);
    if (strcmp(dado->attackType, "NADA CONSTA") != 0)
        fwrite(dado->attackType, sizeof(char), strlen(dado->attackType), pontArq);
    if (strcmp(dado->targetIndustry, "NADA CONSTA") != 0)
        fwrite(dado->targetIndustry, sizeof(char), strlen(dado->targetIndustry), pontArq);
    if (strcmp(dado->defenseMechanism, "NADA CONSTA") != 0)
        fwrite(dado->defenseMechanism, sizeof(char), strlen(dado->defenseMechanism), pontArq);

    // Escreve o lixo no arquivo binário
    for (int i = 0; i < lixo; i++)
    {
        char lixoChar = '$';
        fwrite(&lixoChar, sizeof(char), 1, pontArq);
    }

    return;
}

/*dado_remover():
Remove um dado num arq. bin. no byteOffset indicado
Parâmetro: ponteiro para arquivo, byteOffset do dado
Retorna: true se bem sucedido, mal senão
*/
bool dado_remover(FILE *pontArq, HEADER *headerArq, long int byteOffset)
{
    if (pontArq == NULL || byteOffset < BYTEOFFSET_HEADER)
        return false; // Erro

    // Lendo header do arquivo caso um não seja fornecido
    if (headerArq == NULL)
    {
        // Posicionando ponteiro do arq. no início
        fseek(pontArq, 0, SEEK_SET);
        headerArq = header_ler(pontArq, headerArq);
    }

    // Campos do header
    long int topo = header_get_longint(headerArq, 1);
    long int prox = -1;
    int quantRegArq = header_get_int(headerArq, 1);
    int quantRegRem = header_get_int(headerArq, 2);

    // Lendo dado a ser removido
    DADO *dado = NULL;
    dado = dado_ler(pontArq, dado, byteOffset);
    if (dado == NULL)
    {
        header_apagar(&headerArq);
        return false;
    }

    dado = dado_set(dado, 1, -2, topo, -2, -2, -2,
                    NULL, NULL, NULL, NULL);
    // Atualiza o topo para o byteOffset do registro removido
    topo = byteOffset;

    // Atualizando campos do header
    quantRegArq--; // Decrementa o número de registros no arquivo
    quantRegRem++; // Incrementa o número de registros removidos
    headerArq = header_set(headerArq, 0, topo, -2, quantRegArq, quantRegRem,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // Escrever o dado atualizado no arquivo
    // Posiciona o ponteiro do arquivo no byteOffset do registro removido
    fseek(pontArq, byteOffset, SEEK_SET);
    // Escreve o dado atualizado no arquivo
    dado_escrever(pontArq, dado, 0);

    // Escreve o header atualizado no arquivo
    fseek(pontArq, 0, SEEK_SET);
    headerArq = header_set(headerArq, 1, -2, -2, -2, -2,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    header_escrever(pontArq, headerArq, false);

    dado_apagar(&dado);

    return true;
}