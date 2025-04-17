#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
