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
Cria uma struct do tipo HEADER e a inicializa
Parâmetros: ponteiros para strings (descrições do header)
Retorna: ponteiro para a struct do tipo header
*/
HEADER *header_criar(char *descIdent, char *descYear, char *descFinLoss, char *descCountry,
                     char *descType, char *descTargInd, char *descDef);

/* header_apagar():
Desaloca uma struct do tipo header e define seu ponteiro para NULL
Parâmetros: ponteiro de ponteiro para a struct a ser desalocada
*/
void header_apagar(HEADER **header);

/* header_set_status():
Define o campo de status de um header pré-existente
Parâmetros: ponteiro para o header, status a ser definido
*/
void header_set_status(HEADER *header, char status);

/* header_set_proxByteOffset()
Define o campo proxByteOffset de um header
Parâmetros: ponteiro para header, valor do próximo byte offset livre
*/
void header_set_proxByteOffset(HEADER *header, long int proxByOff);

/* header_set_nroRegArq()
Define o campo nroRegArq de um header
Parâmetros: ponteiro para header, quantidade de registros no arquivos
Retorno: false se header nulo, true caso contrário
*/
void header_set_nroRegArq(HEADER *header, int nroRegAq);

/* header_get_nroRegArq()
Retorna o valor do campo nroRegArq de uma struct header
Parâmetros: ponteiro para struct do tipo header
Retorna: valor do campo nroRegArq da struct (-1 se header == NULL)
*/
int header_get_nroRegArq(HEADER *header);

/* header_get_topo():
Retorna o valor do campo topo de uma struct header
Parâmetros: ponteiro para struct do tipo header
Retorna: valor do campo topo da struct (-1 se header == NULL)
*/
long int header_get_topo(HEADER *header);

/* header_set_topo():
Define o campo topo de um header pré-existente
Parâmetros: ponteiro para o header, valor do topo a ser definido
*/
void header_set_topo(HEADER *header, long int topo);

/* header_get_nroRegRem():
Retorna o valor do campo nroRegRem de uma struct header
Parâmetros: ponteiro para struct do tipo header
Retorna: valor do campo nroRegRem da struct (-1 se header == NULL)
*/
int header_get_nroRegRem(HEADER *header);

/* header_set_nroRegRem():
Define o campo nroRegRem de um header pré-existente
Parâmetros: ponteiro para o header, quantidade de registros removidos a ser definida
*/
void header_set_nroRegRem(HEADER *header, int nroRegRem);

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
Parâmetros: ponteiro para um arquivo, ponteiro para um header e valor booleano (true = escrever string semanticas, false = escrever somente struct)
*/
void header_escrever(FILE *pontArq, HEADER *header, bool semantico);

#endif