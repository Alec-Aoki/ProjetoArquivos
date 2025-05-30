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
DADO *dado_criar(int removido, int tamReg, long int prox, int idAttack, int year, float finLoss,
                 char *country, char *attackType, char *targetInd, char *defMec);

/* dado_apagar():
Desaloca memória da struct e dos campos de tamanho variável
Parâmetro: ponteiro para ponteiro da struct
*/
void dado_apagar(DADO **registro);

/* dado_get_tamReg():
Retorna o valor do campo tamReg de um dado
Parâmetros: ponteiro pra struct do tipo dado
Retorna: valor do campo (-1 se dado nulo)
*/
int dado_get_tamReg(DADO *dado);

/* dado_get_removido():
Retorna o valor do campo removido de um dado
Parâmetros: ponteiro pra struct do tipo dado
Retorna: valor do campo ('a' se dado nulo)
*/
char dado_get_removido(DADO *dado);

/* dado_get_string():
Retorna a string de um campo
Parâmetros: ponteiro pra struct do tipo dado, inteiro de 1 a 4 (campo)
    1: country
    2: attackType
    3: targetIndustry
    4: defenseMechanism
Retorna: ponteiro para string (NULL se não encontrado ou dado nulo)
*/
char *dado_get_string(DADO *dado, int campo);

/* dado_imprimir():
Imprime um dado usando as descrições semânticas do header
Parâmetros: ponteiro para o header, ponteiro para o dado
*/
void dado_imprimir(HEADER *header, DADO *dado);

/* dado_ler():
Lê um registro do arquivo e guarda numa struct DADO
Parâmetros: Ponteiro para arquivo, ponteiro para struct DADO, byteOffset do registro
Retorna: Ponteiro para struct DADO
*/
DADO *dado_ler(FILE *pontArq, DADO *dado, int byteOffset);

/*dado_escrever():
Escreve os campos de uma struct dado em um arquivo
Parâmetros: ponteiro para arquivo, ponteiro para uma struct dado
*/
void dado_escrever(FILE *pontArqBin, DADO *dado);

#endif