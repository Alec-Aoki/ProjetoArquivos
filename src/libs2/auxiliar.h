/*
Contém funções auxilires que não utilizam ponteiros
para arquivos ou structs do tipo HEADER ou DADO
*/

#ifndef AUXILIAR_H
#define AUXILIAR_H

/* str_to_int():
A função converte uma string numérica para um inteiro
Parâmetros: ponteiro para uma string
Retorno : o inteiro correspondente, ou -1 caso a string seja nula
*/
int str_to_int(char *str);

/* str_to_float():
A função converte uma string numérica para um float
Parâmetros: ponteiro para uma string
Retorno : o float correspondente, ou -1 caso a string seja nula
*/
float str_to_float(char *str);

/* formata_string_registro():
Aloca dinamicamente memória para uma string e adiciona delimitadores no inicio e final
Parâmetros: dois ponteiros para string (um a string em si, a outra o id dessa string)
Retorna: ponteiro para string formatada
*/
char *formata_string_registro(char *string, char *id);

/* separa_campo():
A partir de um ponteiro que aponta para o inicio de uma string do registro, separa os campos dela
Parâmetro: ponteiro para ponteiro de char
Retorna: uma string
*/
char *separa_campo(char **pontStr, int id);
#endif