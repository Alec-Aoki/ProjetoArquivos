/*
Lida com a manipulação direta dos campos da struct
de tipo HEADER
*/

#ifndef HEADER_H
#define HEADER_H

// Tamanhos fixos dos campos do header (em bytes)
#define TAM_DESC_ID 23
#define TAM_DESC_YEAR 27
#define TAM_DESC_FIN_LOSS 28
#define TAM_DESC_COUNTRY 26
#define TAM_DESC_TYPE 38
#define TAM_DESC_TGT_IND 38
#define TAM_DESC_DEF 67
#define BYTEOFFSET_HEADER 276

typedef struct header_ HEADER;

/* header_criar():
Aloca memória para uma struct do tipo HEADER e a inicializa
Retorna: ponteiro para a struct do tipo header
*/
HEADER *header_criar();

/* header_set():
Define os campos de uma struct do tipo header.
Se uma struct não for fornecida, cria uma.
Caso -2 ou NULL seja fornecido, não altera o campo da struct.
Parâmetros: valores para os campos do header
Retorna: ponteiro para a struct do tipo header
*/
HEADER *header_set(HEADER *header, int status, long int topo,
                   long int proxByteOffset, int nroRegArq,
                   int nroRegRem, char *descIdent,
                   char *descYear, char *descFinLoss,
                   char *descCountry, char *descType,
                   char *descTargInd, char *descDef);

/* header_apagar():
Desaloca uma struct do tipo header e define seu ponteiro para NULL
Parâmetros: ponteiro de ponteiro para a struct a ser desalocada
*/
void header_apagar(HEADER **header);

/* header_get_int():
Retorna o valor de um campo int
Parâmetros: ponteiro pra struct do tipo header, inteiro de 1 a 2 (campo)
    1: nroRegArq
    2: nroRegRem
Retorna: valor do campo (-1 se não encontrado ou header nulo)
*/
int header_get_int(HEADER *header, int campo);

/* header_get_longint():
Retorna o valor de um campo long int
Parâmetros: ponteiro pra struct do tipo header, inteiro de 1 a 2 (campo)
    1: topo
    2: proxByteOffset
Retorna: valor do campo (-1 se não encontrado ou header nulo)
*/
long int header_get_longint(HEADER *header, int campo);

/* header_get_descricao():
Retorna a string "descreve" de um campo
Parâmetros: ponteiro pra struct do tipo header, inteiro de 1 a 7 (campo)
    1: descreveIdentificador
    2: descreveYear
    3: descreveFinancialLoss
    4: descrevCountry
    5: descreveType
    6: descreveTargetIndustry
    7: descreveDefense
Retorna: ponteiro para string (NULL se não encontrado ou header nulo)
*/
char *header_get_descricao(HEADER *header, int campo);

/* header_ler():
Lê os campos do header de um arquivo binário e guarda em uma struct do tipo HEADER
Parâmetros: ponteiro para arquivo, ponteiro para header
Retorna: ponteiro para header
*/
HEADER *header_ler(FILE *pontArq, HEADER *header);

/* header_escrever():
Escreve um header passado no arquivo binário
Parâmetros: ponteiro para um arquivo, ponteiro para um header e valor booleano
(true = escrever string semanticas, false = escrever somente struct)
*/
void header_escrever(FILE *pontArq, HEADER *header, bool semantico);

#endif