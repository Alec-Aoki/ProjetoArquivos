#ifndef ARVB_H
#define ARVB_H

#define quantMaxFilhos 3 // Ordem 3
#define quantMaxChaves 2
#define TAM_REGISTRO_ARVB 40 // Tamanho fixo do registro de nó na árvore B
#define TAM_HEADER_ARVB 13   // Tamanho fixo do header da árvore B

typedef struct header_arvB_ HEADER_ARVB;

typedef struct no_ NO;

/* ArvB_header_criar():
Aloca memória para uma struct do tipo HEADER_ARVB e a inicializa
Retorna: ponteiro para a struct do tipo HEADER_ARVB
*/
HEADER_ARVB *ArvB_header_criar();

/* ArvB_header_set():
Define os campos de uma struct do tipo HEADER_ARVB.
Se uma struct não for fornecida, cria uma.
Caso -2 seja fornecido, não altera o campo da struct.
Parâmetros: valores para os campos do header
Retorna: ponteiro para a struct do tipo HEADER_ARVB
*/
HEADER_ARVB *ArvB_header_set(HEADER_ARVB *headerArvB, int status,
                             int noRaiz, int proxRRN, int nroNos);

/* ArvB_header_apagar():
Desaloca a memória ocupada por uma struct do tipo HEADER_ARVB.
Parâmetros: ponteiro de ponteiro para a struct do tipo HEADER_ARVB a ser desalocada
*/
void ArvB_header_apagar(HEADER_ARVB **headerArvB);

/* ArvB_header_get_int():
Retorna o valor de um campo inteiro de uma struct do tipo HEADER_ARVB.
Parâmetros: ponteiro para a struct e o campo desejado
    1: noRaiz
    2: proxRRN
    3: nroNos
Retorna: valor do campo (-1 se não encontrado ou header nulo)
*/
int ArvB_header_get_int(HEADER_ARVB *headerArvB, int campo);

/* ArvB_header_ler():
Lê os campos de um header do arquivo binário e guarda em uma struct do tipo HEADER_ARVB
Parâmetros: ponteiro para um arquivo, ponteiro para um header
Retorna: ponteiro para header
*/
HEADER_ARVB *ArvB_header_ler(FILE *pontArq, HEADER_ARVB *headerArvB);

/* ArvB_header_escrever():
Escreve um header passado no arquivo binário
Parâmetros: ponteiro para um arquivo, ponteiro para um header
*/
void ArvB_header_escrever(FILE *pontArq, HEADER_ARVB *headerArvB);

/* ArvB_no_criar():
Aloca memória para uma struct do tipo NO e a inicializa
Retorna: ponteiro para a struct do tipo NO
*/
NO *ArvB_no_criar();

/* ArvB_no_apagar():
Desaloca a memória ocupada por uma struct do tipo NO.
Parâmetros: ponteiro de ponteiro para a struct do tipo NO a ser desalocada
Retorna: ponteiro para a struct do tipo NO (NULL após desalocação)
*/
NO *ArvB_no_apagar(NO **no);

/* ArvB_no_ler():
Lê um nó do arquivo de índice árvore-B
Parâmetros: ponteiro para o arquivo, byteOffset do nó a ser lido
Retorna: ponteiro para a struct do tipo NO lida (NULL se falhar)
*/
NO *ArvB_no_ler(FILE *pontArq, int byteOffset);

/* ArvB_no_set():
Define os campos de uma struct do tipo NO.
Se uma struct não for fornecida, cria uma.
Caso -2 ou NULL seja fornecido, não altera o campo da struct.
Parâmetros: valores para os campos do nó
Retorna: ponteiro para a struct do tipo NO
*/
NO *ArvB_no_set(NO *no, int byteOffset, int *chaves,
                int *byteOffsetDados, int *byteOffsetDescendentes,
                int tipoNo, int quantChavesAtual);

/* ArvB_no_escrever():
Escreve um nó no arquivo de índice árvore-B
Parâmetros: ponteiro para o arquivo, ponteiro para o nó a ser escrito
*/
void ArvB_no_escrever(FILE *pontArq, NO *no);

/*TODO*/
/* ArvB_busca():
Busca uma chave na árvore-B
Parâmetros: ponteiro para o arquivo, byteOffset do nó atual, chave a ser buscada
Retorna: ponteiro para o nó que contém a chave (NULL se não encontrado)
*/
NO *ArvB_busca(FILE *pontArq, int byteOffsetAtual, int chave);

#endif