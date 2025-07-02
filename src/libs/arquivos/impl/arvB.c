#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../registros/header.h"
#include "../../registros/dados.h"
#include "../../auxiliar/auxiliar.h"
#include "../../registros/busca.h"
#include "../arvB.h"

struct header_arvB_
{
    char status; // 0 = inconsistente, 1 = consistente
    int noRaiz;  // byteOffset
    int proxRRN; // byteOffset
    int nroNos;
};

struct no_
{
    int byteOffset;                             // byteOffset atual do reg.
    int chaves[quantMaxChaves];                 // idAttacks
    int byteOffsetDados[quantMaxChaves];        // byteOffsets no arq. de dados
    int byteOffsetDescendentes[quantMaxFilhos]; // byteOffsets dos nós filhos
    int tipoNo;                                 // -1 = folha, 0 = raiz, 1 = interm.
    int quantChavesAtual;
};

/* ArvB_header_criar():
Aloca memória para uma struct do tipo HEADER_ARVB e a inicializa
Retorna: ponteiro para a struct do tipo HEADER_ARVB
*/
HEADER_ARVB *ArvB_header_criar()
{
    HEADER_ARVB *novoHeaderArvB = (HEADER_ARVB *)malloc(sizeof(HEADER_ARVB));
    if (novoHeaderArvB == NULL)
    {
        mensagem_erro();
        return NULL;
    }

    /*INICIALIZANDO O HEADER*/
    novoHeaderArvB->status = '1'; // Consistente
    novoHeaderArvB->noRaiz = -1;  // Sem raiz inicial
    novoHeaderArvB->proxRRN = 0;  // Próximo RRN a ser usado
    novoHeaderArvB->nroNos = 0;   // Inicialmente sem nós

    return novoHeaderArvB;
}

/* ArvB_header_set():
Define os campos de uma struct do tipo HEADER_ARVB.
Se uma struct não for fornecida, cria uma.
Caso -2 seja fornecido, não altera o campo da struct.
Parâmetros: valores para os campos do header
Retorna: ponteiro para a struct do tipo HEADER_ARVB
*/
HEADER_ARVB *ArvB_header_set(HEADER_ARVB *headerArvB, int status,
                             int noRaiz, int proxRRN, int nroNos)
{
    if (headerArvB == NULL)
        return NULL;

    if (status != -2)
        headerArvB->status = status + '0'; // Conversão int -> char
    if (noRaiz != -2)
        headerArvB->noRaiz = noRaiz;
    if (proxRRN != -2)
        headerArvB->proxRRN = proxRRN;
    if (nroNos != -2)
        headerArvB->nroNos = nroNos;

    return headerArvB;
}

/* ArvB_header_apagar():
Desaloca a memória ocupada por uma struct do tipo HEADER_ARVB.
Parâmetros: ponteiro de ponteiro para a struct do tipo HEADER_ARVB a ser desalocada
*/
void ArvB_header_apagar(HEADER_ARVB **headerArvB)
{
    if (headerArvB == NULL || *headerArvB == NULL)
        return;

    free(*headerArvB);  // Desalocando memória
    *headerArvB = NULL; // Definindo o ponteiro para NULL
}

/* ArvB_header_get_int():
Retorna o valor de um campo inteiro de uma struct do tipo HEADER_ARVB.
Parâmetros: ponteiro para a struct e o campo desejado
    1: noRaiz
    2: proxRRN
    3: nroNos
Retorna: valor do campo (-1 se não encontrado ou header nulo)
*/
int ArvB_header_get_int(HEADER_ARVB *headerArvB, int campo)
{
    if (headerArvB == NULL)
        return -1;

    switch (campo)
    {
    case 1: // noRaiz
        return headerArvB->noRaiz;
    case 2: // proxRRN
        return headerArvB->proxRRN;
    case 3: // nroNos
        return headerArvB->nroNos;
    default:
        break;
    }

    return -1; // Campo inválido
}

/* ArvB_header_ler():
Lê os campos de um header do arquivo binário e guarda em uma struct do tipo HEADER_ARVB
Parâmetros: ponteiro para um arquivo, ponteiro para um header
Retorna: ponteiro para header
*/
HEADER_ARVB *ArvB_header_ler(FILE *pontArq, HEADER_ARVB *headerArvB)
{
    if (pontArq == NULL)
        return NULL; // Erro

    // Criando uma nova struct do tipo header caso uma não seja fornecida
    if (headerArvB == NULL)
    {
        headerArvB = ArvB_header_criar();
        if (headerArvB == NULL)
            return NULL;
    }

    fseek(pontArq, 0, SEEK_SET); // Posicionando ponteiro no início do arquivo

    // Lendo os campos do header
    fread(&(headerArvB->status), sizeof(char), 1, pontArq);
    fread(&(headerArvB->noRaiz), sizeof(int), 1, pontArq);
    fread(&(headerArvB->proxRRN), sizeof(int), 1, pontArq);
    fread(&(headerArvB->nroNos), sizeof(int), 1, pontArq);

    return headerArvB;
}

/* ArvB_header_escrever():
Escreve um header passado no arquivo binário
Parâmetros: ponteiro para um arquivo, ponteiro para um header
*/
void ArvB_header_escrever(FILE *pontArq, HEADER_ARVB *headerArvB)
{
    if (pontArq == NULL || headerArvB == NULL)
        return;

    // Escrevendo os campos do header no arquivo
    fwrite(&(headerArvB->status), sizeof(char), 1, pontArq);
    fwrite(&(headerArvB->noRaiz), sizeof(int), 1, pontArq);
    fwrite(&(headerArvB->proxRRN), sizeof(int), 1, pontArq);
    fwrite(&(headerArvB->nroNos), sizeof(int), 1, pontArq);

    return;
}