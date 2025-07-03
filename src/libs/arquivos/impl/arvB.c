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
    {
        headerArvB = ArvB_header_criar();
        if (headerArvB == NULL)
            return NULL;
    }

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

    // Colocando o ponteiro no início do arquivo
    fseek(pontArq, 0, SEEK_SET);

    // Escrevendo os campos do header no arquivo
    fwrite(&(headerArvB->status), sizeof(char), 1, pontArq);
    fwrite(&(headerArvB->noRaiz), sizeof(int), 1, pontArq);
    fwrite(&(headerArvB->proxRRN), sizeof(int), 1, pontArq);
    fwrite(&(headerArvB->nroNos), sizeof(int), 1, pontArq);

    return;
}

/* ArvB_no_criar():
Aloca memória para uma struct do tipo NO e a inicializa
Retorna: ponteiro para a struct do tipo NO
*/
NO *ArvB_no_criar()
{
    NO *novoNo = (NO *)malloc(sizeof(NO));
    if (novoNo == NULL)
    {
        mensagem_erro();
        return NULL;
    }

    // Inicializando o nó
    novoNo->byteOffset = -1; // Inicialmente sem byteOffset
    for (int i = 0; i < quantMaxChaves; i++)
    {
        novoNo->chaves[i] = -1;          // Sem chaves
        novoNo->byteOffsetDados[i] = -1; // Sem byteOffsets de dados
    }
    for (int i = 0; i < quantMaxFilhos; i++)
        novoNo->byteOffsetDescendentes[i] = -1; // Sem descendentes

    novoNo->tipoNo = -1;          // Inicialmente folha
    novoNo->quantChavesAtual = 0; // Inicialmente sem chaves

    return novoNo;
}

/* ArvB_no_apagar():
Desaloca a memória ocupada por uma struct do tipo NO.
Parâmetros: ponteiro de ponteiro para a struct do tipo NO a ser desalocada
Retorna: ponteiro para a struct do tipo NO (NULL após desalocação)
*/
NO *ArvB_no_apagar(NO **no)
{
    if (no == NULL || *no == NULL)
        return NULL;

    free(*no);  // Desalocando memória
    *no = NULL; // Definindo o ponteiro para NULL

    return *no;
}

/* ArvB_no_ler():
Lê um nó do arquivo de índice árvore-B
Parâmetros: ponteiro para o arquivo, byteOffset do nó a ser lido
Retorna: ponteiro para a struct do tipo NO lida (NULL se falhar)
*/
NO *ArvB_no_ler(FILE *pontArq, int byteOffset)
{
    if (pontArq == NULL || byteOffset < 0)
    {
        mensagem_erro();
        return NULL;
    }

    NO *no = ArvB_no_criar();
    if (no == NULL)
    {
        mensagem_erro();
        return NULL;
    }

    // Posicionando ponteiro no byteOffset do nó
    fseek(pontArq, byteOffset, SEEK_SET);
    // Lendo os campos do nó
    fread(&(no->byteOffset), sizeof(int), 1, pontArq);
    fread(no->chaves, sizeof(int), quantMaxChaves, pontArq);
    fread(no->byteOffsetDados, sizeof(int), quantMaxChaves, pontArq);
    fread(no->byteOffsetDescendentes, sizeof(int), quantMaxFilhos, pontArq);
    fread(&(no->tipoNo), sizeof(int), 1, pontArq);
    fread(&(no->quantChavesAtual), sizeof(int), 1, pontArq);

    return no;
}

/* ArvB_no_set():
Define os campos de uma struct do tipo NO.
Se uma struct não for fornecida, cria uma.
Caso -2 ou NULL seja fornecido, não altera o campo da struct.
Parâmetros: valores para os campos do nó
Retorna: ponteiro para a struct do tipo NO
*/
NO *ArvB_no_set(NO *no, int byteOffset, int *chaves,
                int *byteOffsetDados, int *byteOffsetDescendentes,
                int tipoNo, int quantChavesAtual)
{
    if (no == NULL)
        no = ArvB_no_criar();

    if (byteOffset != -2)
        no->byteOffset = byteOffset;
    if (chaves != NULL)
        memcpy(no->chaves, chaves, sizeof(int) * quantMaxChaves);
    if (byteOffsetDados != NULL)
        memcpy(no->byteOffsetDados, byteOffsetDados, sizeof(int) * quantMaxChaves);
    if (byteOffsetDescendentes != NULL)
        memcpy(no->byteOffsetDescendentes, byteOffsetDescendentes, sizeof(int) * quantMaxFilhos);
    if (tipoNo != -2)
        no->tipoNo = tipoNo;
    if (quantChavesAtual != -2)
        no->quantChavesAtual = quantChavesAtual;

    return no;
}

/* ArvB_no_escrever():
Escreve um nó no arquivo de índice árvore-B
Parâmetros: ponteiro para o arquivo, ponteiro para o nó a ser escrito
*/
void ArvB_no_escrever(FILE *pontArq, NO *no)
{
    if (pontArq == NULL || no == NULL)
    {
        mensagem_erro();
        return;
    }

    // Posicionando ponteiro no byteOffset do nó
    fseek(pontArq, no->byteOffset, SEEK_SET);

    // Escrevendo os campos do nó no arquivo
    fwrite(&(no->byteOffset), sizeof(int), 1, pontArq);
    fwrite(no->chaves, sizeof(int), quantMaxChaves, pontArq);
    fwrite(no->byteOffsetDados, sizeof(int), quantMaxChaves, pontArq);
    fwrite(no->byteOffsetDescendentes, sizeof(int), quantMaxFilhos, pontArq);
    fwrite(&(no->tipoNo), sizeof(int), 1, pontArq);
    fwrite(&(no->quantChavesAtual), sizeof(int), 1, pontArq);

    return;
}

int ArvB_no_get_int(NO *no, int campo)
{
    if (no == NULL)
        return -1;

    switch (campo)
    {
    case 1: // byteOffset
        return no->byteOffset;
    case 2: // tipoNo
        return no->tipoNo;
    case 3: // quantChavesAtual
        return no->quantChavesAtual;
    default:
        break;
    }

    return -1; // Campo inválido
}

/*TODO*/
/* ArvB_busca():
Busca uma chave na árvore-B
Parâmetros: ponteiro para o arquivo, byteOffset do nó atual, chave a ser buscada
Retorna: ponteiro para o nó que contém a chave (NULL se não encontrado)
*/
NO *ArvB_busca(FILE *pontArq, int byteOffsetAtual, int chave)
{
    if (pontArq == NULL || byteOffsetAtual < 0)
    {
        mensagem_erro();
        return NULL;
    }

    NO *noAtual = NULL;
    int i = 0;

    fseek(pontArq, byteOffsetAtual, SEEK_SET);
    noAtual = ArvB_no_ler(pontArq, byteOffsetAtual);
    if (noAtual == NULL)
    {
        mensagem_erro();
        return NULL;
    }

    // Procura a chave no nó atual, avançando o índice quando a chave for maior que as chaves do nó
    while (i < noAtual->quantChavesAtual && chave > noAtual->chaves[i])
    {
        i++;
    }

    // Verifica se a chave foi encontrada
    if (i < noAtual->quantChavesAtual && chave == noAtual->chaves[i])
    {
        return noAtual; // Chave encontrada
    }

    // Se não encontrou, verifica se é uma folha
    if (noAtual->tipoNo == -1)
    {
        ArvB_no_apagar(&noAtual); // Libera memória do nó atual
        return NULL;
    }
    else
    {
        int byteOffsetFilho = noAtual->byteOffsetDescendentes[i];
        ArvB_no_apagar(&noAtual); // Libera memória do nó atual

        // Se a chave não foi encontrada, segue para o filho correspondente
        return ArvB_busca(pontArq, byteOffsetFilho, chave);
    }
}

/* inserir_ordenado():
Insere uma chave, o byteOffset do seu dado e o byteOffset de seu nó filho
nos vetores de um nó ordenadamente. Caso o byteOffset do nó filho seja -2,
ele não será inserido
Parâmetros: ponteiro para nó, chave, byteOffset do dado e byteOffste do nó filho
*/
void inserir_ordenado(NO *no, int chave, int byteOffsetDado, int byteOffsetFilho)
{
    if (no == NULL || chave < 0 || byteOffsetDado < 0)
        return; // Erro

    // Encontrando posição do campo novo
    int pos = 0;
    while (chave > no->chaves[pos] && pos < quantMaxChaves)
        pos++;

    if (no->chaves[pos] == chave)
        return; // Chave já inserida

    // Shiftando os campos já existentes pra direita
    for (int i = no->quantChavesAtual; i > pos; i--)
    {
        no->chaves[i] = no->chaves[i - 1];
        no->byteOffsetDados[i] = no->byteOffsetDados[i - 1];
        if (byteOffsetFilho != -2)
            no->byteOffsetDescendentes[i + 1] = no->byteOffsetDescendentes[i];
    }

    // Inserindo campos
    no->chaves[pos] = chave;
    no->byteOffsetDados[pos] = byteOffsetDado;
    if (byteOffsetFilho != -2)
        no->byteOffsetDescendentes[pos + 1] = byteOffsetFilho;

    no->quantChavesAtual++;

    return;
}

/*TODO*/

void ArvB_inserir_recursivo(FILE *pontArq, HEADER_ARVB *header, int chave, int byteOffsetDado, int byteOffsetNoAtual);

/* ArvB_inserir():
Função inicial para mexer no header e lidar com a primeira inserção
Parâmetros: ponteiro para header de arvB, chave a ser inserido, byteOffset do
registro no arquivo de dados com essa chave
*/
void ArvB_inserir(FILE *pontArq, HEADER_ARVB *header, int chave, int byteOffsetDado)
{
    if (pontArq == NULL || header == NULL)
        return; // Erro

    int byteOffsetNoRaiz = header->noRaiz;

    // Caso 1: árvore vázia
    if (byteOffsetNoRaiz == -1)
    {
        NO *noRaiz = NULL;
        noRaiz = ArvB_no_set(noRaiz, TAM_HEADER_ARVB + (header->proxRRN) * TAM_REGISTRO_ARVB, NULL, NULL, NULL, 0, 1);
        if (noRaiz == NULL)
            return;

        noRaiz->chaves[0] = chave;
        noRaiz->byteOffsetDados[0] = byteOffsetDado;

        // Preenchendo o resto dos campos como nulo
        for (int i = 1; i < quantMaxChaves; i++)
        {
            noRaiz->chaves[i] = -1;
            noRaiz->byteOffsetDados[i] = -1;
        }
        for (int i = 0; i < quantMaxFilhos; i++)
        {
            noRaiz->byteOffsetDescendentes[i] = -1;
        }

        // Definindo campos do header antes da escrita
        header = ArvB_header_set(header, 0, noRaiz->byteOffset, 1, 1);

        // Escrevendo header e nó raíz
        ArvB_header_escrever(pontArq, header);
        ArvB_no_escrever(pontArq, noRaiz);

        // Definindo header como consistente e escrevendo
        header->status = 1;
        ArvB_header_escrever(pontArq, header);

        ArvB_no_apagar(&noRaiz);
        return;
    }

    // Caso 2: árvore não-vazia
}

/* ArvB_inserir_recursivo():
Função para lidar com a inserção no caso em que a árvore não está vazia (caso geral)
Parâmetros: ponteiro para header de arvB, chave a ser inserido, byteOffset do
registro no arquivo de dados com essa chave, byteOffset do nó sendo lido
*/

void ArvB_inserir_recursivo(FILE *pontArq, HEADER_ARVB *header, int chave, int byteOffsetDado, int byteOffsetNoAtual)
{
    if (pontArq == NULL || header == NULL)
        return; // Erro

    // Lendo nó atual
    NO *noAtual = NULL;
    noAtual = ArvB_no_ler(pontArq, byteOffsetNoAtual);
    if (noAtual == NULL)
        return;

    // Caso 1: nó folha
    if (noAtual->tipoNo == -1)
    {
        // Caso tenha espaço no nó folha
        if (noAtual->quantChavesAtual < quantMaxChaves)
        {
            inserir_ordenado(noAtual, chave, byteOffsetDado, -2);
            ArvB_no_escrever(pontArq, noAtual);
        }
        else
        {
            // Split
        }
    }
    else
    {
        // Caso 2: nó intermediário, continuar procurando nó folha
    }
}