/*
Lida com a manipulação direta dos campos da struct
de tipo DADO
*/

#ifndef DADOS_H
#define DADOS_H

typedef struct dados_ DADO;

/* dado_criar():
Aloca memória para uma struct do tipo dado e inicializa seus campos
Retorna: ponteiro para dado
*/
DADO *dado_criar();

/* dado_set():
Define campos de uma struct dado. Caso uma struct não seja fornecida, cria uma
Parâmetros: ponteiro para dado valores dos campos da struct
Retorna: ponteiro para dado
*/
DADO *dado_set(DADO *dado, int removido, int tamReg, long int prox, int idAttack, int year, float finLoss,
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

/* dado_set_removido():
Define o valor do campo removido de um dado
Parâmetros: ponteiro pra struct do tipo dado, char removido
Retorna: void
*/
void dado_set_removido(DADO *dado, char removido);

/* dado_get_prox():
Retorna o valor do campo prox de um dado
Parâmetros: ponteiro pra struct do tipo dado
Retorna: valor do campo (-1 se dado nulo)
*/
long int dado_get_prox(DADO *dado);

/* dado_set_prox():
Define o valor do campo prox de um dado
Parâmetros: ponteiro pra struct do tipo dado, long int prox
Retorna: void
*/
void dado_set_prox(DADO *dado, long int prox);

/* dado_set_tamReg():
Define o valor do campo tamReg de um dado
Parâmetros: ponteiro pra struct do tipo dado, inteiro tamReg
Retorna: void
*/
void dado_set_tamReg(DADO *dado, int tamReg);

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

/* dado_get_int():
Retorna o valor de um campo do tipo int
Parâmetros: ponteiro pra struct do tipo dado, inteiro de 1 a 2 (campo)
    1: idAttack
    2: year
Retorna: valor do campo (-1 se mal sucedido)
*/
int dado_get_int(DADO *dado, int campo);

/* dado_get_finLoss():
Retorna o valor do campo financialLoss
Parâmetros: ponteiro pra struct do tipo dado
Retorna: valor do campo (-1 se mal sucedido)
*/
float dado_get_finLoss(DADO *dado);

/* dado_imprimir():
Imprime um dado usando as descrições semânticas do header
Parâmetros: ponteiro para o header, ponteiro para o dado
*/
void dado_imprimir(HEADER *header, DADO *dado);

/* dado_ler():
Lê um registro do arquivo e guarda numa struct DADO
Parâmetros: Ponteiro para arquivo, ponteiro para struct DADO, byteOffset do registro
Retorna: ponteiro para dado
*/
DADO *dado_ler(FILE *pontArq, DADO *dado, int byteOffset);

/*dado_escrever():
Escreve os campos de uma struct dado em um arquivo
Parâmetros: ponteiro para arquivo, ponteiro para uma struct dado
*/
void dado_escrever(FILE *pontArqBin, DADO *dado);

/* dado_escrever_lixo():
Escreve um dado em um arquivo, mas com dados inválidos ou lixo
Parâmetros: ponteiro para arquivo, ponteiro para uma struct dado, inteiro que indica a quantidade de lixo a ser escrito
Retorna: void
*/
void dado_escrever_lixo(FILE *pontArqBin, DADO *dado, int lixo);


#endif