#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../auxiliar/auxiliar.h"
#include "../header.h"
#include "../dados.h"
#include "../busca.h"

/*Armazena campos de uma busca*/
struct busca_
{
    int quantCampos;    // Quantidade de campos que serão pesquisados
    int quaisCampos[7]; // Quant. máx. de campos = 7
    int idAttack;
    int year;
    float finLoss;
    char country[TAM_MAX_STR];
    char attackType[TAM_MAX_STR];
    char targetIndustry[TAM_MAX_STR];
    char defenseMechanism[TAM_MAX_STR];
};

/* busca_criar():
Aloca memória para uma struct do tipo busca e a inicializa
Retorna: ponteiro para struct (NULL se mal sucedido)
*/
BUSCA *busca_criar()
{
    BUSCA *novaBusca = (BUSCA *)malloc(sizeof(BUSCA));
    if (novaBusca == NULL)
        return NULL;

    novaBusca->quantCampos = 0;
    for (int i = 0; i < 7; i++)
    {
        novaBusca->quaisCampos[i] = -1;
    }

    novaBusca->idAttack = -1;
    novaBusca->year = -1;
    novaBusca->finLoss = -1;
    strcpy(novaBusca->country, "$");
    strcpy(novaBusca->attackType, "$");
    strcpy(novaBusca->targetIndustry, "$");
    strcpy(novaBusca->defenseMechanism, "$");

    return novaBusca;
}

/* busca_apagar():
Desaloca uma struct do tipo busca
Parâmetro: ponteiro duplo para struct do tipo busca
*/
void busca_apagar(BUSCA **busca)
{
    if (*busca == NULL)
        return;

    free(*busca);
    *busca = NULL;

    return;
}

/* busca_ler():
Lê um input do usuário e preenche uma struct busca. Se a struct não existir, cria uma
Parâmetros: ponteiro para struct busca
Retorna: ponteiro para struct busca (NULL se mal sucedido)
*/
BUSCA *busca_ler(BUSCA *busca)
{
    if (busca == NULL)
        busca = busca_criar();

    char buffer[256];                   // Buffer para leitura de strings
    int byteOffset = BYTEOFFSET_HEADER; // Byteoffset inicial para leitura dos dados do arquivo
    char *ptr;

    int flag = -1;  // Indicador de qual campo deve ser buscado
    int j = 0;      // Contador para quantCampos
    int tamStr = 0; // Auxiliar

    scanf("%d", &(busca->quantCampos)); // Recebendo do usuário quantas respostas (dados) devemos imprimir

    // Lendo string contendo os campos a serem buscados e seus valores
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    ptr = buffer;

    /*Lendo qual campo devemos buscar, salvando essa informação no vetor quais campos
        e guardando seus valores nas variáveis auxiliares corretas*/
    char *tok = strsep(&ptr, " ");
    while ((tok = strsep(&ptr, " ")) != NULL && j < busca->quantCampos)
    { // Loop enquanto não chegarmos ao fim da string ou da quantidade de campos
        // Determina qual campo deve ser buscado
        if (strcmp(tok, "idAttack") == 0)
            flag = 0;
        else if (strcmp(tok, "year") == 0)
            flag = 1;
        else if (strcmp(tok, "financialLoss") == 0)
            flag = 2;
        else if (strcmp(tok, "country") == 0)
            flag = 3;
        else if (strcmp(tok, "attackType") == 0)
            flag = 4;
        else if (strcmp(tok, "targetIndustry") == 0)
            flag = 5;
        else if (strcmp(tok, "defenseMechanism") == 0)
            flag = 6;

        busca->quaisCampos[j] = flag; // Guardando esse campo no vetor

        // Avançando o ponteiro na string
        tok = strsep(&ptr, " "); // Tok aponta para o valor do campo que acaba de ser guardado
        if (tok == NULL)
            break; // Caso tenhamos chegado ao final da string

        // Atribuindo esse valor para a variável auxilar correta
        switch (flag)
        {
        case 0:
            busca->idAttack = str_to_int(tok);
            break;
        case 1:
            busca->year = str_to_int(tok);
            break;
        case 2:
            busca->finLoss = str_to_float(tok);
            break;
        case 3:
            tamStr = strlen(tok) - 2;
            strncpy(busca->country, tok + 1, tamStr);
            busca->country[tamStr] = '\0';
            break;
        case 4:
            tamStr = strlen(tok) - 2;
            strncpy(busca->attackType, tok + 1, tamStr);
            busca->attackType[tamStr] = '\0';
            break;
        case 5:
            tamStr = strlen(tok) - 2;
            strncpy(busca->targetIndustry, tok + 1, tamStr);
            busca->targetIndustry[tamStr] = '\0';
            break;
        case 6:
            tamStr = strlen(tok) - 2;
            strncpy(busca->defenseMechanism, tok + 1, tamStr);
            busca->defenseMechanism[tamStr] = '\0';
            break;
        }

        j++; // Avançando o contador para a quantidade de campos e voltando ao início do while
    }

    return busca;
}

/* busca_comparar():
Compara uma struct dado com os parâmetros de uma struct busca
Parâmetros: ponteiro para struct busca, ponteiro para struct dado
Retorna: booleano (true se forem iguais, falso se não)
*/
bool busca_comparar(BUSCA *busca, DADO *dado)
{
    if (busca == NULL || dado == NULL)
        return false;

    bool dadoValido = true;

    // Loop para verificar se todos os campos são obedecidos, percorrendo o vetor quaisCampos para isso
    for (int k = 0; k < busca->quantCampos; k++)
    {
        switch (busca->quaisCampos[k])
        {
        // Caso algum dos campos não satisfaça a busca, a flag é settada para false
        case 0: // idAttack
            if (dado_get_int(dado, 1) != busca->idAttack)
                dadoValido = false;
            break;
        case 1: // year
            if (dado_get_int(dado, 2) != busca->year)
                dadoValido = false;
            break;
        case 2: // financialLoss
            if (dado_get_finLoss(dado) != busca->finLoss)
                dadoValido = false;
            break;
        case 3: // country
            if (strcmp(dado_get_string(dado, 1), busca->country) != 0)
                dadoValido = false;
            break;
        case 4: // attackType
            if (strcmp(dado_get_string(dado, 2), busca->attackType) != 0)
                dadoValido = false;
            break;
        case 5: // targetIndustry
            if (strcmp(dado_get_string(dado, 3), busca->targetIndustry) != 0)
                dadoValido = false;
            break;
        case 6: // defenseMechanism
            if (strcmp(dado_get_string(dado, 4), busca->defenseMechanism) != 0)
                dadoValido = false;
            break;
        default:
            break;
        }
    }

    return dadoValido;
}