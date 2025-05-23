#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../auxiliar.h"

int str_to_int(char *str)
{
    if (str == NULL)
        return -1; // Erro

    if (strcmp(str, "") == 0)
        return -1; // Erro

    return atoi(str);
}

float str_to_float(char *str)
{
    if (str == NULL)
        return -1; // Erro

    if (strcmp(str, "") == 0)
        return -1; // Erro

    return atof(str);
}

char *formata_string_registro(char *string, char *id)
{
    // Caso de campos nulos
    if (string == NULL || strcmp(string, "") == 0)
    {
        return NULL;
    }

    // Aloca memória para a string com o tamanho extra para os delimitadores
    char *strTemp = (char *)malloc(sizeof(char) * (strlen(string) + strlen(id) + 2));
    if (strTemp == NULL)
        return NULL; // Erro de alocação de memória

    // Construção segura da string
    strTemp[0] = '\0'; // Inicializa o buffer
    strcat(strTemp, id);
    strcat(strTemp, string);
    strcat(strTemp, "|");

    return strTemp; // Retorna a string formatada
}

char *separa_campo(char **pontStr, int id)
{
    if (**pontStr != (id + '0'))
    {
        return strdup("NADA CONSTA");
    }
    // Recebe um ponteiro para a primeira ocorrecia do caractere dado (Fim da string)
    char *lugarDelimitador = strchr(*pontStr, '|');
    // Aponta para o inicio pulando o ID
    char *inicio = *pontStr + 1;

    // Calcula do tamanho da string
    int tamStr = lugarDelimitador - inicio;

    // Aloca memória para string e verifica se ocorreu corretamente
    char *campo = (char *)malloc(tamStr + 1);
    if (campo == NULL)
        return NULL;

    // Copia a string e adiciona o caractere terminador
    memcpy(campo, inicio, tamStr);
    campo[tamStr] = '\0';

    // Avança o ponteiro para o próximo campo
    *pontStr = lugarDelimitador + 1;

    // retorna a string sem id nem delimitador
    return campo;
}