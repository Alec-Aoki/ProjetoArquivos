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

    novaBusca->idAttack = -2;
    novaBusca->year = -2;
    novaBusca->finLoss = -2;
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

/* busca_get_quaisCampos():
Retorna o valor de um campo do vetor quaisCampos
de uma struct busca
Parâmetros: ponteiro para struct, inteiro (campo)
Retorna: inteiro
*/
int busca_get_quaisCampos(BUSCA *busca, int campo)
{
    if (busca == NULL)
        return -2;

    return busca->quaisCampos[campo];
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

    // Buffer para leitura de strings
    char buffer[256];
    // Byteoffset inicial para leitura dos dados do arquivo
    int byteOffset = BYTEOFFSET_HEADER;
    char *ptr;

    int flag = -1;  // Indicador de qual campo deve ser buscado
    int j = 0;      // Contador para quantCampos
    int tamStr = 0; // Auxiliar

    // Recebendo do usuário quantas campos teremos na busca
    scanf("%d", &(busca->quantCampos));

    // Lendo string contendo os campos a serem buscados e seus valores
    fgets(buffer, sizeof(buffer), stdin);
    // Localiza no buffer o primeiro caractere de nova linha(\n) ou retorno de carro(\r)
    buffer[strcspn(buffer, "\n\r")] = '\0';

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
        // Tok aponta para o valor do campo que acaba de ser guardado
        tok = strsep(&ptr, " ");
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
            if (strcmp(tok, "NULO") == 0)
                strcpy(busca->country, "NULO");
            else
            {
                tamStr = strlen(tok) - 2;
                strncpy(busca->country, tok + 1, tamStr);
                busca->country[tamStr] = '\0';
                strcpy(busca->country, formata_string_registro(busca->country, "1"));
                limpa_barra_final(busca->country);
            }
            break;
        case 4:
            if (strcmp(tok, "NULO") == 0)
                strcpy(busca->attackType, "NULO");
            else
            {
                tamStr = strlen(tok) - 2;
                strncpy(busca->attackType, tok + 1, tamStr);
                busca->attackType[tamStr] = '\0';
                strcpy(busca->attackType, formata_string_registro(busca->attackType, "2"));
                limpa_barra_final(busca->attackType);
            }
            break;
        case 5:
            if (strcmp(tok, "NULO") == 0)
                strcpy(busca->targetIndustry, "NULO");
            else
            {
                tamStr = strlen(tok) - 2;
                strncpy(busca->targetIndustry, tok + 1, tamStr);
                busca->targetIndustry[tamStr] = '\0';
                strcpy(busca->targetIndustry, formata_string_registro(busca->targetIndustry, "3"));
                limpa_barra_final(busca->targetIndustry);
            }
            break;
        case 6:
            if (strcmp(tok, "NULO") == 0)
                strcpy(busca->defenseMechanism, "NULO");
            else
            {
                tamStr = strlen(tok) - 2;
                strncpy(busca->defenseMechanism, tok + 1, tamStr);
                busca->defenseMechanism[tamStr] = '\0';
                strcpy(busca->defenseMechanism, formata_string_registro(busca->defenseMechanism, "4"));
                limpa_barra_final(busca->defenseMechanism);
            }
            break;
        }
        // Avançando o contador para a quantidade de campos e voltando ao início do while
        j++;
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

    // Loop para verificar se todos os campos são obedecidos,
    // percorrendo o vetor quaisCampos para isso
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
        {
            char *dadoCountry = dado_get_string(dado, 1);
            char tempDado[256], tempBusca[256];
            strcpy(tempDado, dadoCountry);
            strcpy(tempBusca, busca->country);
            limpa_barra_final(tempDado);
            limpa_barra_final(tempBusca);
            if (strcmp(tempDado, tempBusca) != 0)
                dadoValido = false;
        }
        break;
        case 4: // attackType
        {
            char *dadoAttackType = dado_get_string(dado, 2);
            char tempDado[256], tempBusca[256];
            strcpy(tempDado, dadoAttackType);
            strcpy(tempBusca, busca->attackType);
            limpa_barra_final(tempDado);
            limpa_barra_final(tempBusca);
            if (strcmp(tempDado, tempBusca) != 0)
                dadoValido = false;
        }
        break;
        case 5: // targetIndustry
        {
            char *dadoTargetIndustry = dado_get_string(dado, 3);
            char tempDado[256], tempBusca[256];
            strcpy(tempDado, dadoTargetIndustry);
            strcpy(tempBusca, busca->targetIndustry);
            limpa_barra_final(tempDado);
            limpa_barra_final(tempBusca);
            if (strcmp(tempDado, tempBusca) != 0)
                dadoValido = false;
        }
        break;
        case 6: // defenseMechanism
        {
            char *dadoDefenseMechanism = dado_get_string(dado, 4);
            char tempDado[256], tempBusca[256];
            strcpy(tempDado, dadoDefenseMechanism);
            strcpy(tempBusca, busca->defenseMechanism);
            limpa_barra_final(tempDado);
            limpa_barra_final(tempBusca);
            if (strcmp(tempDado, tempBusca) != 0)
                dadoValido = false;
        }
        break;
        default:
            break;
        }
    }

    return dadoValido;
}

/*busca_atualizar_dado():
Atualiza uma struct dado pré-existente com os campos de uma struct busca
Parâmetros: ponteiro para struct busca, ponteiro para struct dado
Retorna: ponteiro para dado
*/
DADO *busca_atualizar_dado(BUSCA *busca, DADO *dado)
{
    if (busca == NULL || dado == NULL)
        return NULL;

    dado = dado_set(dado, -2, -2, -2, busca->idAttack, busca->year, busca->finLoss,
                    desformata_string_registro(busca->country),
                    desformata_string_registro(busca->attackType),
                    desformata_string_registro(busca->targetIndustry),
                    desformata_string_registro(busca->defenseMechanism));

    dado_atualizar_tamReg(dado);

    return dado;
}

/* busca_get_int():
Retorna o valor de um campo inteiro de uma struct busca
Parâmetros: ponteiro para struct busca, inteiro (campo)
Retorna: valor do campo ou -2 se inválido
*/
int busca_get_int(BUSCA *busca, int campo)
{
    if (busca == NULL)
        return -2;

    switch (campo)
    {
    case 0: // idAttack
        return busca->idAttack;
    case 1: // year
        return busca->year;
    case 2: // financialLoss
        return busca->finLoss;
    default:
        return -2; // Campo inválido
    }
}

BUSCA *busca_set(BUSCA *busca, int idAttack, int year, float finLoss,
                 char *country, char *attackType, char *targetIndustry, char *defenseMechanism)
{
    if (busca == NULL)
        busca = busca_criar();
    if (busca == NULL)
        return NULL;

    int i = 0;
    if (idAttack > 0)
    {
        busca->quantCampos++;
        busca->idAttack = idAttack;
        busca->quaisCampos[i++] = 0; // idAttack
    }
    if (year > 0)
    {
        busca->quantCampos++;
        busca->year = year;
        busca->quaisCampos[i++] = 1; // year
    }
    if (finLoss > 0)
    {
        busca->quantCampos++;
        busca->finLoss = finLoss;
        busca->quaisCampos[i++] = 2; // financialLoss
    }
    if (country != NULL && strcmp(country, "NULO") != 0)
    {
        busca->quantCampos++;
        strcpy(busca->country, country);
        busca->quaisCampos[i++] = 3; // country
    }
    if (attackType != NULL && strcmp(attackType, "NULO") != 0)
    {
        busca->quantCampos++;
        strcpy(busca->attackType, attackType);
        busca->quaisCampos[i++] = 4; // attackType
    }
    if (targetIndustry != NULL && strcmp(targetIndustry, "NULO") != 0)
    {
        busca->quantCampos++;
        strcpy(busca->targetIndustry, targetIndustry);
        busca->quaisCampos[i++] = 5; // targetIndustry
    }
    if (defenseMechanism != NULL && strcmp(defenseMechanism, "NULO") != 0)
    {
        busca->quantCampos++;
        strcpy(busca->defenseMechanism, defenseMechanism);
        busca->quaisCampos[i++] = 6; // defenseMechanism
    }

    return busca;
}

void busca_imprimir(BUSCA *busca)
{
    if (busca == NULL)
        return;

    printf("Busca com %d campos:\n", busca->quantCampos);
    for (int i = 0; i < busca->quantCampos; i++)
    {
        switch (busca->quaisCampos[i])
        {
        case 0:
            printf("idAttack: %d\n", busca->idAttack);
            break;
        case 1:
            printf("year: %d\n", busca->year);
            break;
        case 2:
            printf("financialLoss: %.2f\n", busca->finLoss);
            break;
        case 3:
            printf("country: %s\n", busca->country);
            break;
        case 4:
            printf("attackType: %s\n", busca->attackType);
            break;
        case 5:
            printf("targetIndustry: %s\n", busca->targetIndustry);
            break;
        case 6:
            printf("defenseMechanism: %s\n", busca->defenseMechanism);
            break;
        default:
            break;
        }
    }
    printf("\n");
}