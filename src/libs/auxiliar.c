#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "auxiliar.h"

//  Funções auxiliares
    
/* str_to_int():
A função converte uma string numérica para um intero, caso nula -1
Parâmetros: uma string
Retorno : o inteiro correspondente, ou -1 
*/
int str_to_int (char *str) {
    if (str == NULL)
        return -1;
    
    if (strcmp(str, "") == 0)
        return -1;

    return atoi(str);
}
    
/* str_to_float():
A função converte uma string numérica para um float, caso nula -1
Parâmetros: uma string
Retorno : o float correspondente, ou -1 
*/
float str_to_float (char *str) {
    if (str == NULL)
        return -1;
    
    if (strcmp(str, "") == 0)
        return -1;

    return atof(str);    
}

/* formata_string_registro():
Aloca dinamicamente memória para uma string e adiciona delimitadores no inicio e final
Parâmetros: string a ser formatada
Retorna: uma string formatada  
*/
char *formata_string_registro (char *string, char *id){
    

    // Aloca memória para a string com o tamanho extra para os delimitadores
    char *strTemp = (char *) malloc(sizeof(char)*(strlen(string)+strlen(id)+2));
    if (strTemp == NULL) return NULL;

    if (string == NULL || strcmp(string, "") == 0){
        strcpy(strTemp, "|");
        return strTemp;
    }
    
    // Construção segura da string
    strTemp[0] = '\0'; // Inicializa o buffer
    strcat(strTemp, id);
    strcat(strTemp, string);
    strcat(strTemp, "|");

    return strTemp; // Retorna a string formatada
}

/* separa_campo():
A partir de um ponteiro que aponta para o inicio de uma string do registro separa os campos dela
Parâmetro: Ponteiro para ponteiro de char
Retorna: string
*/
char *separa_campo (char **pontStr) {
    // recebe um ponteiro para a primeira ocorrecia do caractere dado (Fim da string)
    char *lugarDelimitador = strchr(*pontStr, '|');
    // Aponta para o inicio pulando o ID
    char *inicio = *pontStr + 1;

    // Calcula do tamanho da string
    int tamStr = lugarDelimitador - inicio; 
    
    // Caso campo nulo
    if (tamStr <= 0) {
        *pontStr = lugarDelimitador + 1;
        return strdup("NADA CONSTA");
    }

    // Aloca memória para string e verifica se ocorreu corretamente
    char *campo = (char *) malloc(tamStr+1);
    if (campo == NULL) return NULL;

    // Copia a string e adiciona o caractere terminador
    memcpy(campo, inicio, tamStr);
    campo[tamStr] = '\0';

    // Avança o ponteiro para o próximo campo
    *pontStr = tamStr+1;

    // retorna a string sem id nem delimitador
    return campo;
}