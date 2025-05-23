/*
Lida com a manipulação direta dos campos da struct
de tipo DADO
*/

#ifndef DADOS_H
#define DADOS_H

#define TAM_MAX_STR 256

typedef struct dados_ DADO;

/* dado_criar():
Aloca memória para uma struct do tipo dado e inicializa seus campos
Parâmetros: valores dos campos da struct
Retorna: ponteiro para dado
*/
DADO *dado_criar(int removido, int tamReg, long int prox, int idAttack, int year, float finLoss, char *country, char *attackType, char *targetInd, char *defMec);

/* dado_apagar():
Desaloca memória da struct e dos campos de tamanho variável
Parâmetro: ponteiro para ponteiro da struct
*/
void dado_apagar(DADO **registro);

/* dado_imprimir():
Imprime um dado usando as descrições semânticas do header
Parâmetros: ponteiro para o header, ponteiro para o dado
*/
void dado_imprimir(HEADER *header, DADO *dado);

/* dado_get_string():
Retorna a string de um campo
Parâmetros: ponteiro pra struct do tipo dado, inteiro de 1 a 4 (campo)
Retorna: ponteiro para string (NULL se não encontrado ou dado nulo)
*/
char *dado_get_string(DADO *dado, int campo);
 
#endif