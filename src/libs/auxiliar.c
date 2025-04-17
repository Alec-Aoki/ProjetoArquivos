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
    if (string == NULL || strcmp(string, "") == 0) return "|";

    // Aloca memória para a string com o tamanho extra para os delimitadores
    char *strTemp = (char *) malloc(sizeof(char)*(strlen(string)+strlen(id)+2));
    if (strTemp == NULL) return NULL;
    
    // Construção segura da string
    strTemp[0] = '\0'; // Inicializa o buffer
    strcat(strTemp, id);
    strcat(strTemp, string);
    strcat(strTemp, "|");

    return strTemp; // Retorna a string formatada
}