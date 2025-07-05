#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../registros/header.h"
#include "../../registros/dados.h"
#include "../../auxiliar/auxiliar.h"
#include "../../registros/busca.h"
#include "../../auxiliar/auxiliar.h"

#include "../arvB.h"

struct header_arvB_
{
    char status; // 0 = inconsistente, 1 = consistente
    int noRaiz;  // RRN do nó raiz
    int proxRRN;
    int nroNos;
};

struct no_
{
    long int byteOffset; // byteOffset atual do reg./nó

    int quantChavesAtual;
    int tipoNo;                               // -1 = folha, 0 = raiz, 1 = interm.
    int chaves[quantMaxChaves];               // idAttacks
    long int byteOffsetDados[quantMaxChaves]; // byteOffsets no arq. de dados
    int rrnDescendentes[quantMaxFilhos];      // RRN dos nós filhos
};

typedef struct promocao_
{
    NO *noNovo;
    int chavePromov;
    long int byteOffsetDadoPromov;
    bool houveSplit;
} PROMOCAO;

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
    4: status
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
    case 4: // status
        return headerArvB->status - '0';
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
    fwrite("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$", sizeof(char), 31, pontArq);
    fflush(pontArq);

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
        novoNo->rrnDescendentes[i] = -1; // Sem descendentes

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
    no->byteOffset = byteOffset;
    // Lendo os campos do nó
    fread(&(no->tipoNo), sizeof(int), 1, pontArq);
    fread(&(no->quantChavesAtual), sizeof(int), 1, pontArq);
    fread(&(no->rrnDescendentes[0]), sizeof(int), 1, pontArq);
    fread(&(no->chaves[0]), sizeof(int), 1, pontArq);
    fread(&(no->byteOffsetDados[0]), sizeof(long int), 1, pontArq);
    fread(&(no->rrnDescendentes[1]), sizeof(int), 1, pontArq);
    fread(&(no->chaves[1]), sizeof(int), 1, pontArq);
    fread(&(no->byteOffsetDados[1]), sizeof(long int), 1, pontArq);
    fread(&(no->rrnDescendentes[2]), sizeof(int), 1, pontArq);

    return no;
}

/* ArvB_no_set():
Define os campos de uma struct do tipo NO.
Se uma struct não for fornecida, cria uma.
Caso -2 ou NULL seja fornecido, não altera o campo da struct.
Parâmetros: valores para os campos do nó
Retorna: ponteiro para a struct do tipo NO
*/
NO *ArvB_no_set(NO *no, long int byteOffset, int *chaves,
                long int *byteOffsetDados, int *rrnDescendentes,
                int tipoNo, int quantChavesAtual)
{
    if (no == NULL)
        no = ArvB_no_criar();

    if (byteOffset != -2)
        no->byteOffset = byteOffset;
    if (chaves != NULL)
        memcpy(no->chaves, chaves, sizeof(int) * quantMaxChaves);
    if (byteOffsetDados != NULL)
        memcpy(no->byteOffsetDados, byteOffsetDados, sizeof(long int) * quantMaxChaves);
    if (rrnDescendentes != NULL)
        memcpy(no->rrnDescendentes, rrnDescendentes, sizeof(int) * quantMaxFilhos);
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

    if (no->byteOffset < TAM_HEADER_ARVB)
        return; // Erro
    // Posicionando ponteiro no byteOffset do nó
    fseek(pontArq, no->byteOffset, SEEK_SET);

    // Escrevendo os campos do nó no arquivo
    fwrite(&(no->tipoNo), sizeof(int), 1, pontArq);
    fwrite(&(no->quantChavesAtual), sizeof(int), 1, pontArq);
    fwrite(&(no->rrnDescendentes[0]), sizeof(int), 1, pontArq);
    fwrite(&(no->chaves[0]), sizeof(int), 1, pontArq);
    fwrite(&(no->byteOffsetDados[0]), sizeof(long int), 1, pontArq);
    fwrite(&(no->rrnDescendentes[1]), sizeof(int), 1, pontArq);
    fwrite(&(no->chaves[1]), sizeof(int), 1, pontArq);
    fwrite(&(no->byteOffsetDados[1]), sizeof(long int), 1, pontArq);
    fwrite(&(no->rrnDescendentes[2]), sizeof(int), 1, pontArq);

    return;
}

/* ArvB_no_get_int():
Retorna o valor de um campo inteiro de uma struct do tipo no.
Parâmetros: ponteiro para a struct e o campo desejado
    1: tipoNO
    2: quantChavesAtual
    3: chave 1
    4: chave 2
    5: rrnFilho 1
    6: rrnFilho 2
    7: rrnFIlho 3
Retorna: valor do campo (-1 se não encontrado ou no nulo)
*/
int ArvB_no_get_int(NO *no, int campo)
{
    if (no == NULL)
        return -1;

    switch (campo)
    {
    case 1: // tipoNo
        return no->tipoNo;
    case 2: // quantChavesAtual
        return no->quantChavesAtual;
    case 3:
        return no->chaves[0];
    case 4:
        return no->chaves[1];
    case 5:
        return no->rrnDescendentes[0];
    case 6:
        return no->rrnDescendentes[1];
    case 7:
        return no->rrnDescendentes[2];
    default:
        break;
    }

    return -1; // Campo inválido
}

/* ArvB_no_get_longint():
Retorna o valor de um campo long int de uma struct do tipo no.
Parâmetros: ponteiro para a struct e o campo desejado
    1: byteOffset do nó
    2: byteOffsetDado chave 1
    3: byteOffsetDado chave2
Retorna: valor do campo (-1 se não encontrado ou no nulo)
*/
long int ArvB_no_get_longint(NO *no, int campo)
{
    if (no == NULL)
        return -1;

    switch (campo)
    {
    case 1: // Byteoffset nó
        return no->byteOffset;
    case 2: // Chave 1
        return no->byteOffsetDados[0];
    case 3: // Chave 2
        return no->byteOffsetDados[1];
    default:
        break;
    }

    return -1; // Campo inválido
}

/* ArvB_busca():
Busca uma chave na árvore-B
Parâmetros: ponteiro para o arquivo, byteOffset do nó atual, chave a ser buscada
Retorna: ponteiro para o nó que contém a chave (NULL se não encontrado)
*/
NO *ArvB_busca(FILE *pontArq, long int byteOffsetAtual, int chave)
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
        i++;

    // Verifica se a chave foi encontrada
    if (i < noAtual->quantChavesAtual && chave == noAtual->chaves[i])
        return noAtual; // Chave encontrada

    // Se não encontrou, verifica se é uma folha
    if (noAtual->tipoNo == -1)
    {
        ArvB_no_apagar(&noAtual); // Libera memória do nó atual
        return NULL;
    }
    else
    {
        int byteOffsetFilho = TAM_HEADER_ARVB + noAtual->rrnDescendentes[i] * TAM_REGISTRO_ARVB;
        ArvB_no_apagar(&noAtual); // Libera memória do nó atual

        // Se a chave não foi encontrada, segue para o filho correspondente
        return ArvB_busca(pontArq, byteOffsetFilho, chave);
    }

    return NULL;
}

// Funções internas auxiliares da inserção:
int inserir_ordenado(int *chaves, long int *byteOffsetDados, int *rrnDescendentes,
                     int *quantChavesAtual, int chave, long int byteOffsetDado, int rrnFilho);
PROMOCAO ArvB_inserir_recursivo(FILE *pontArq, HEADER_ARVB *header, int chave, long int byteOffsetDado, long int byteOffsetNoAtual);
PROMOCAO ArvB_split(NO *no, HEADER_ARVB *header, int chave, long int byteOffsetDado, int rrnFilho);

/* ArvB_inserir():
Função "macro" para mexer no header e lidar com a primeira inserção
Parâmetros: ponteiro para arquivo, ponteiro para header de arvB, chave a ser inserido e seus campos
*/
void ArvB_inserir(FILE *pontArq, HEADER_ARVB *header, int chave, long int byteOffsetDado)
{
    if (pontArq == NULL || header == NULL)
        return; // Erro

    int byteOffsetNoRaiz = TAM_HEADER_ARVB + header->noRaiz * TAM_REGISTRO_ARVB;

    // Caso 1: árvore vázia
    if (header->noRaiz == -1)
    {
        NO *noRaiz = NULL;
        noRaiz = ArvB_no_set(noRaiz, TAM_HEADER_ARVB + (header->proxRRN) * TAM_REGISTRO_ARVB, NULL, NULL, NULL, -1, 1);
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
            noRaiz->rrnDescendentes[i] = -1;
        }

        // Definindo campos do header
        header->noRaiz = 0;
        header->proxRRN = 1;
        header->nroNos = 1;

        // Escrevendo nó raíz
        ArvB_no_escrever(pontArq, noRaiz);

        ArvB_no_apagar(&noRaiz);
    }

    else
    {
        // Caso 2: árvore não-vazia
        PROMOCAO promocao = ArvB_inserir_recursivo(pontArq, header, chave, byteOffsetDado, byteOffsetNoRaiz);

        if (promocao.houveSplit)
        {
            NO *raizNova = ArvB_no_criar();
            if (raizNova == NULL)
                return;

            raizNova->byteOffset = TAM_HEADER_ARVB + header->proxRRN * TAM_REGISTRO_ARVB;
            raizNova->tipoNo = 0; // Raíz
            raizNova->quantChavesAtual = 1;
            raizNova->chaves[0] = promocao.chavePromov;
            raizNova->byteOffsetDados[0] = promocao.byteOffsetDadoPromov;
            raizNova->rrnDescendentes[0] = (byteOffsetNoRaiz - TAM_HEADER_ARVB) / TAM_REGISTRO_ARVB;
            raizNova->rrnDescendentes[1] = (promocao.noNovo->byteOffset - TAM_HEADER_ARVB) / TAM_REGISTRO_ARVB;

            // Inicializando os campos restantes
            for (int i = 1; i < quantMaxChaves; i++)
            {
                raizNova->chaves[i] = -1;
                raizNova->byteOffsetDados[i] = -1;
            }
            for (int i = 2; i < quantMaxFilhos; i++)
                raizNova->rrnDescendentes[i] = -1;

            // Atualizando os tipos dos nós filhos
            NO *noRaizAntigo = ArvB_no_ler(pontArq, byteOffsetNoRaiz);
            if (noRaizAntigo != NULL)
            {
                int tipoNo = -1; // Folha
                for (int i = 0; i < quantMaxFilhos; i++)
                {
                    if (noRaizAntigo->rrnDescendentes[i] != -1)
                    {
                        tipoNo = 1; // Intermediario
                        break;
                    }
                }
                noRaizAntigo->tipoNo = tipoNo;
                ArvB_no_escrever(pontArq, noRaizAntigo);
                ArvB_no_apagar(&noRaizAntigo);
            }

            int tipoNo = -1; // Folha
            for (int i = 0; i < quantMaxFilhos; i++)
            {
                if (promocao.noNovo->rrnDescendentes[i] != -1)
                {
                    tipoNo = 1; // Intermediario
                    break;
                }
            }
            promocao.noNovo->tipoNo = tipoNo;
            ArvB_no_escrever(pontArq, promocao.noNovo);

            ArvB_no_escrever(pontArq, raizNova); // Escrevendo raíz nova

            // Atualizando header
            header->noRaiz = header->proxRRN;
            header->proxRRN++;
            header->nroNos++;

            ArvB_no_apagar(&raizNova);
            ArvB_no_apagar(&(promocao.noNovo));
        }
    }
    return;
}

/* ArvB_inserir_recursivo():
Caso "geral" da inserção, que busca pela nó em que a chave será inserida usando recursão
Parâmetros: ponteiro para arquivo, ponteiro para header de arvB, chave a ser inserido e seus campos
Retorna: struct da chave que foi promovida (se houve split)
*/
PROMOCAO ArvB_inserir_recursivo(FILE *pontArq, HEADER_ARVB *header, int chave, long int byteOffsetDado, long int byteOffsetNoAtual)
{
    PROMOCAO promocao;
    promocao.noNovo = NULL;
    promocao.chavePromov = -1;
    promocao.byteOffsetDadoPromov = -1;
    promocao.houveSplit = false;

    if (pontArq == NULL || header == NULL || byteOffsetNoAtual < 0)
        return promocao; // Erro

    // Lendo nó atual
    NO *noAtual = NULL;
    noAtual = ArvB_no_ler(pontArq, byteOffsetNoAtual);
    if (noAtual == NULL)
        return promocao;

    // Caso 1: nó folha
    if (noAtual->tipoNo == -1)
    {
        // Caso 1: tem espaço no nó folha
        if (noAtual->quantChavesAtual < quantMaxChaves)
        {
            inserir_ordenado(noAtual->chaves, noAtual->byteOffsetDados, NULL, &(noAtual->quantChavesAtual), chave, byteOffsetDado, -2);
            ArvB_no_escrever(pontArq, noAtual);

            promocao.houveSplit = false;
        }
        else
        {
            // Caso 2: não tem espaço no nó folha, split
            promocao = ArvB_split(noAtual, header, chave, byteOffsetDado, -2);

            ArvB_no_escrever(pontArq, noAtual);         // Atualizando nó original
            ArvB_no_escrever(pontArq, promocao.noNovo); // Nó resultando do split

            header->proxRRN++;
            header->nroNos++;
        }
    }
    else
    {
        // Caso 2: nó intermediário, continuar procurando nó folha
        int pos = 0;
        while (pos < noAtual->quantChavesAtual && chave > noAtual->chaves[pos])
            pos++;

        // Chave já inserida
        if (chave == noAtual->chaves[pos])
        {
            ArvB_no_apagar(&noAtual);
            return promocao;
        }

        // Faz a busca a partir do nó filho (recursão)
        PROMOCAO promocaoFilho = ArvB_inserir_recursivo(pontArq, header, chave, byteOffsetDado, TAM_HEADER_ARVB + noAtual->rrnDescendentes[pos] * TAM_REGISTRO_ARVB);

        // Caso 1: houve split e promoção
        if (promocaoFilho.houveSplit)
        {
            // Caso 1: o nó atual tem espaço pra chave que foi promovida
            if (noAtual->quantChavesAtual < quantMaxChaves)
            {
                inserir_ordenado(noAtual->chaves, noAtual->byteOffsetDados, noAtual->rrnDescendentes, &(noAtual->quantChavesAtual),
                                 promocaoFilho.chavePromov, promocaoFilho.byteOffsetDadoPromov, (promocaoFilho.noNovo->byteOffset - TAM_HEADER_ARVB) / TAM_REGISTRO_ARVB);

                ArvB_no_escrever(pontArq, noAtual);

                ArvB_no_apagar(&(promocaoFilho.noNovo));

                promocao.houveSplit = false;
            }
            else // Caso 2: não tem espaço, split
            {
                promocao = ArvB_split(noAtual, header, promocaoFilho.chavePromov, promocaoFilho.byteOffsetDadoPromov,
                                      (promocaoFilho.noNovo->byteOffset - TAM_HEADER_ARVB) / TAM_REGISTRO_ARVB);

                ArvB_no_escrever(pontArq, noAtual);         // Atualizando nó original
                ArvB_no_escrever(pontArq, promocao.noNovo); // Nó resultando do split

                header->proxRRN++;
                header->nroNos++;

                ArvB_no_apagar(&(promocaoFilho.noNovo));
            }
        }
        // Else Caso 2: não houve split, tudo ok
    }

    ArvB_no_apagar(&noAtual);

    return promocao;
}

/* inserir_ordenado():
Insere uma chave em um nó, realizando o split e indicando a chave a ser promovida
Parâmetros: ponteiro para no, chave e campos a serem inseridos
Retorna: struct que indica as informacoes da chave a ser promovida
*/
PROMOCAO ArvB_split(NO *no, HEADER_ARVB *header, int chave, long int byteOffsetDado, int rrnFilho)
{
    PROMOCAO promocao;
    promocao.noNovo = NULL;
    promocao.chavePromov = -1;
    promocao.byteOffsetDadoPromov = -1;
    promocao.houveSplit = false;

    if (no == NULL || header == NULL)
        return promocao; // Erro

    // Vetores temporários
    int chavesTemp[quantMaxChaves + 1];
    long int byteOffsetDadosTemp[quantMaxChaves + 1];
    int rrnFilhosTemp[quantMaxFilhos + 1];
    int quantChavesTemp = quantMaxChaves;

    // Inicializando vetores temporários
    for (int i = 0; i < quantMaxChaves + 1; i++)
    {
        chavesTemp[i] = -1;
        byteOffsetDadosTemp[i] = -1;
    }
    for (int i = 0; i < quantMaxFilhos + 1; i++)
        rrnFilhosTemp[i] = -1;

    // Copiando dados do nó para estes vetores
    for (int i = 0; i < quantMaxChaves; i++)
    {
        chavesTemp[i] = no->chaves[i];
        byteOffsetDadosTemp[i] = no->byteOffsetDados[i];
    }
    for (int i = 0; i < quantMaxFilhos; i++)
        rrnFilhosTemp[i] = no->rrnDescendentes[i];

    // Inserindo a chave nos vetores temporários
    inserir_ordenado(chavesTemp, byteOffsetDadosTemp, rrnFilhosTemp, &quantChavesTemp, chave, byteOffsetDado, rrnFilho);

    // Criando nó novo
    NO *noNovo = ArvB_no_criar();
    if (noNovo == NULL)
        return promocao; // Erro

    // Redistribuindo chaves e etc -> genérico e não hardcoded :D
    int meio = quantChavesTemp / 2;          // Meio do vetor
    promocao.chavePromov = chavesTemp[meio]; // Pega a chave que pode ser promovida (ordem 3 -> pega [1])
    promocao.byteOffsetDadoPromov = byteOffsetDadosTemp[meio];
    promocao.houveSplit = true;

    // Limpando nó original
    for (int i = 0; i < quantMaxChaves; i++)
    {
        no->chaves[i] = -1;
        no->byteOffsetDados[i] = -1;
    }
    for (int i = 0; i < quantMaxFilhos; i++)
        no->rrnDescendentes[i] = -1;

    // Nó esquerdo (original)
    for (int i = 0; i < meio; i++)
    {
        no->chaves[i] = chavesTemp[i];
        no->byteOffsetDados[i] = byteOffsetDadosTemp[i];
    }
    no->quantChavesAtual = meio;

    // Nó direito (novo)
    int j = 0;
    for (int i = meio + 1; i < quantChavesTemp; i++)
    {
        noNovo->chaves[j] = chavesTemp[i];
        noNovo->byteOffsetDados[j] = byteOffsetDadosTemp[i];
        j++;
    }
    noNovo->quantChavesAtual = quantChavesTemp - meio - 1;

    // Redistribuindo os filhos
    // Nó esquerdo
    for (int i = 0; i <= meio; i++)
        no->rrnDescendentes[i] = rrnFilhosTemp[i];

    // Nó direito
    j = 0;
    for (int i = meio + 1; i < quantMaxFilhos + 1; i++)
    {
        noNovo->rrnDescendentes[j] = rrnFilhosTemp[i];
        j++;
    }

    noNovo->byteOffset = TAM_HEADER_ARVB + header->proxRRN * TAM_REGISTRO_ARVB;
    noNovo->tipoNo = no->tipoNo;

    promocao.noNovo = noNovo;

    return promocao;
}

/* inserir_ordenado():
Insere uma chave e os respectivos campos nos vetores de uma estrutura nó.
Não utilizamos a struct para podermos usar essa função no split, cujos vetores
têm tamanho diferentes do nó
Parâmetros: ponteiro para vetores, chave e campos a serem inseridos
Retorna: posição encontrada para a chave
*/
int inserir_ordenado(int *chaves, long int *byteOffsetDados, int *rrnDescendentes,
                     int *quantChavesAtual, int chave, long int byteOffsetDado, int rrnFilho)
{
    if (chaves == NULL || byteOffsetDados == NULL)
        return -1; // Erro

    // Encontrando posição da chave
    int pos = 0;
    while (pos < *quantChavesAtual && chave > chaves[pos])
        pos++;

    // Deslocando chaves e etc. para a direita
    for (int i = *quantChavesAtual; i > pos; i--)
    {
        chaves[i] = chaves[i - 1];
        byteOffsetDados[i] = byteOffsetDados[i - 1];
    }

    // Inserindo a chave na posicao que abrimos
    chaves[pos] = chave;
    byteOffsetDados[pos] = byteOffsetDado;

    if (quantChavesAtual != NULL)
        (*quantChavesAtual)++;

    if (rrnFilho != -2 && rrnDescendentes != NULL)
    {
        // Deslocando pra direita
        for (int i = quantMaxFilhos - 1; i > pos + 1; i--)
            rrnDescendentes[i] = rrnDescendentes[i - 1];

        rrnDescendentes[pos + 1] = rrnFilho;
    }

    return pos;
}

/* ArvB_DFS():
Realiza uma busca em profundidade na árvore B e imprime os dados que satisfazem a busca
Parâmetros: ponteiro para o arquivo, byteOffset atual, ponteiro para a busca e header
*/
void ArvB_DFS(FILE *pontArqArv, FILE *pontArqDados, long int byteOffsetAtual, BUSCA *busca,
              HEADER *header, bool *encontrado, BUSCA *camposAtualizados)
{
    // Condição de parada
    if (byteOffsetAtual < 0)
        return;

    NO *noAtual = NULL;
    noAtual = ArvB_no_ler(pontArqArv, byteOffsetAtual);
    if (noAtual == NULL)
    {
        mensagem_erro();
        return;
    }

    // Se o nó não é folha, intercala a trajetoria
    if (noAtual->tipoNo != -1)
    {
        for (int i = 0; i < noAtual->quantChavesAtual; i++)
        {
            ArvB_DFS(pontArqArv, pontArqDados, TAM_HEADER_ARVB + noAtual->rrnDescendentes[i] * TAM_REGISTRO_ARVB, busca, header, encontrado, camposAtualizados);
            long int byteOffset = noAtual->byteOffsetDados[i];
            if (camposAtualizados == NULL)
            {
                DADO *dado = NULL;
                dado = dado_ler(pontArqDados, dado, byteOffset);
                if (dado == NULL)
                {
                    mensagem_erro();
                    return;
                }

                if (dado_get_removido(dado) == '0') // Verifica se o dado não foi removido
                {
                    if (busca_comparar(busca, dado))
                    {
                        dado_imprimir(header, dado); // Imprime o dado se a busca for bem-sucedida
                        printf("\n");
                        *encontrado = true;
                    }
                }
                dado_apagar(&dado);
            }
            else
            {
                DADO *dado = NULL;
                DADO *dadoAtualizado = NULL;
                dado = dado_ler(pontArqDados, dado, byteOffset);
                dadoAtualizado = dado_ler(pontArqDados, dadoAtualizado, byteOffset);
                if (dado == NULL || dadoAtualizado == NULL)
                {
                    mensagem_erro();
                    return;
                }
                dadoAtualizado = busca_atualizar_dado(camposAtualizados, dadoAtualizado);

                if (dado_get_int(dadoAtualizado, 3) > dado_get_int(dado, 3))
                {
                    dado_remover(pontArqDados, header, byteOffset);

                    arqBIN_insert_dado(pontArqDados, header, dadoAtualizado);
                }
                else
                {
                    // Calculando quantidade de lixo
                    int quantLixo = dado_get_int(dado, 3) - dado_get_int(dadoAtualizado, 3);
                    // Definindo tamanho do dado atualizado para o tam. do dado orig.
                    dadoAtualizado = dado_set(dadoAtualizado, -2, dado_get_int(dado, 3),
                                              -2, -2, -2, -2, NULL, NULL, NULL, NULL);
                    // Posicionando ponteiro no início do dado
                    fseek(pontArqDados, byteOffset, SEEK_SET);
                    // Sobrescrevendo dado
                    dado_escrever(pontArqDados, dadoAtualizado, quantLixo);
                }
                dado_apagar(&dadoAtualizado);
                dado_apagar(&dado);
            }
        }

        ArvB_DFS(pontArqArv, pontArqDados, TAM_HEADER_ARVB + noAtual->rrnDescendentes[noAtual->quantChavesAtual] * TAM_REGISTRO_ARVB, busca, header, encontrado, camposAtualizados);
    }
    // Se o nó é folha, processa o nó
    else
    {
        for (int i = 0; i < noAtual->quantChavesAtual; i++)
        {
            long int byteOffset = noAtual->byteOffsetDados[i];
            if (camposAtualizados == NULL)
            {
                DADO *dado = NULL;
                dado = dado_ler(pontArqDados, dado, byteOffset);
                if (dado == NULL)
                {
                    mensagem_erro();
                    return;
                }

                if (dado_get_removido(dado) == '0') // Verifica se o dado não foi removido
                {
                    if (busca_comparar(busca, dado))
                    {
                        dado_imprimir(header, dado); // Imprime o dado se a busca for bem-sucedida
                        printf("\n");
                        *encontrado = true;
                    }
                }
                dado_apagar(&dado);
            }
            else
            {
                DADO *dado = NULL;
                DADO *dadoAtualizado = NULL;
                dado = dado_ler(pontArqDados, dado, byteOffset);
                dadoAtualizado = dado_ler(pontArqDados, dadoAtualizado, byteOffset);
                if (dado == NULL || dadoAtualizado == NULL)
                {
                    mensagem_erro();
                    return;
                }
                dadoAtualizado = busca_atualizar_dado(camposAtualizados, dadoAtualizado);

                if (dado_get_int(dadoAtualizado, 3) > dado_get_int(dado, 3))
                {
                    dado_remover(pontArqDados, header, byteOffset);

                    arqBIN_insert_dado(pontArqDados, header, dadoAtualizado);
                }
                else
                {
                    // Calculando quantidade de lixo
                    int quantLixo = dado_get_int(dado, 3) - dado_get_int(dadoAtualizado, 3);
                    // Definindo tamanho do dado atualizado para o tam. do dado orig.
                    dadoAtualizado = dado_set(dadoAtualizado, -2, dado_get_int(dado, 3),
                                              -2, -2, -2, -2, NULL, NULL, NULL, NULL);
                    // Posicionando ponteiro no início do dado
                    fseek(pontArqDados, byteOffset, SEEK_SET);
                    // Sobrescrevendo dado
                    dado_escrever(pontArqDados, dadoAtualizado, quantLixo);
                }
                dado_apagar(&dadoAtualizado);
                dado_apagar(&dado);
            }
        }
    }

    return;
}

/* ArvB_compara_dado():
Compara os dados de um nó com uma busca e imprime os dados que satisfazem a busca
Parâmetros: ponteiro para o arquivo, ponteiro para o nó a ser comparado, ponteiro para a busca
*/
void ArvB_compara_dado(FILE *pontArq, NO *no, BUSCA *busca, BUSCA *camposAtualizados, HEADER *headerDados)
{
    if (no == NULL || busca == NULL)
        return; // Erro

    HEADER *header = header_ler(pontArq, header);

    // Percorre as chaves do nó
    for (int i = 0; i < no->quantChavesAtual; i++)
    {
        if (camposAtualizados == NULL)
        {
            DADO *dado = NULL;
            dado = dado_ler(pontArq, dado, no->byteOffsetDados[i]);
            if (dado == NULL)
            {
                mensagem_erro();
                return;
            }

            if (dado_get_removido(dado) == '0') // Verifica se o dado não foi removido
            {
                if (busca_comparar(busca, dado))
                {
                    dado_imprimir(header, dado); // Imprime o dado se a busca for bem-sucedida
                    printf("\n");
                }
            }
            dado_apagar(&dado);
        }
        else
        {
            DADO *dado = NULL;
            DADO *dadoAtualizado = NULL;
            dado = dado_ler(pontArq, dado, no->byteOffsetDados[i]);
            dadoAtualizado = dado_ler(pontArq, dadoAtualizado, no->byteOffsetDados[i]);
            if (dado == NULL || dadoAtualizado == NULL)
            {
                mensagem_erro();
                return;
            }

            dadoAtualizado = busca_atualizar_dado(camposAtualizados, dadoAtualizado);

            // Dado atualizado maior que dado original
            if (dado_get_int(dadoAtualizado, 3) > dado_get_int(dado, 3))
            {
                dado_remover(pontArq, headerDados, no->byteOffsetDados[i]);

                arqBIN_insert_dado(pontArq, headerDados, dadoAtualizado);
                dado_apagar(&dadoAtualizado);
            }
            else
            {
                // Calculando quantidade de lixo
                int quantLixo = dado_get_int(dado, 3) - dado_get_int(dadoAtualizado, 3);
                // Definindo tamanho do dado atualizado para o tam. do dado orig.
                dadoAtualizado = dado_set(dadoAtualizado, -2, dado_get_int(dado, 3),
                                          -2, -2, -2, -2, NULL, NULL, NULL, NULL);
                // Posicionando ponteiro no início do dado
                fseek(pontArq, no->byteOffsetDados[i], SEEK_SET);
                // Sobrescrevendo dado
                dado_escrever(pontArq, dadoAtualizado, quantLixo);
                dado_apagar(&dadoAtualizado);
            }

            dado_apagar(&dado);
        }
    }

    header_apagar(&header);
    return;
}

// Função para debuggar
void print_no(NO *no)
{
    if (no == NULL)
        return;

    printf("**********************\n");
    printf("Byteoffset: %ld\n", no->byteOffset);
    printf("Tipo no: %d\n\n", no->tipoNo);

    printf("Chave 1: %d\n", no->chaves[0]);
    printf("Chave 2: %d\n\n", no->chaves[1]);

    printf("Byteoffset Chave 1: %ld\n", no->byteOffsetDados[0]);
    printf("Byteoffset Chave 2: %ld\n\n", no->byteOffsetDados[1]);

    printf("RRN Filho 1: %d\n", no->rrnDescendentes[0]);
    printf("RRN Filho 2: %d\n", no->rrnDescendentes[1]);
    printf("RRN Filho 3: %d\n", no->rrnDescendentes[2]);
    printf("**********************\n\n");

    return;
}

// Função para debuggar
void print_header(HEADER_ARVB *header)
{
    if (header == NULL)
        return;

    printf("RRN noRaiz: %d\n", header->noRaiz);
    printf("Quant nos: %d\n", header->nroNos);
    printf("ProxRRN: %d\n", header->proxRRN);
    printf("Status: %c\n", header->status);

    return;
}

void print_arvore(FILE *pontArqArv, FILE *pontArqDados, long int byteOffsetAtual, HEADER *header)
{
    // Condição de parada
    if (byteOffsetAtual < 0)
        return;

    NO *noAtual = NULL;
    noAtual = ArvB_no_ler(pontArqArv, byteOffsetAtual);
    if (noAtual == NULL)
    {
        mensagem_erro();
        return;
    }

    // Se o nó não é folha, intercala a trajetoria
    if (noAtual->tipoNo != -1)
    {
        for (int i = 0; i < noAtual->quantChavesAtual; i++)
        {
            print_arvore(pontArqArv, pontArqDados, TAM_HEADER_ARVB + noAtual->rrnDescendentes[i] * TAM_REGISTRO_ARVB, header);
            long int byteOffset = noAtual->byteOffsetDados[i];

            DADO *dado = NULL;
            dado = dado_ler(pontArqDados, dado, byteOffset);
            if (dado == NULL)
            {
                mensagem_erro();
                return;
            }

            if (dado_get_removido(dado) == '0') // Verifica se o dado não foi removido
            {
                dado_imprimir(header, dado); // Imprime o dado se a busca for bem-sucedida
                printf("\n");
            }
            dado_apagar(&dado);
        }

        print_arvore(pontArqArv, pontArqDados, TAM_HEADER_ARVB + noAtual->rrnDescendentes[noAtual->quantChavesAtual] * TAM_REGISTRO_ARVB, header);
    }
    // Se o nó é folha, processa o nó
    else
    {
        for (int i = 0; i < noAtual->quantChavesAtual; i++)
        {
            long int byteOffset = noAtual->byteOffsetDados[i];

            DADO *dado = NULL;
            dado = dado_ler(pontArqDados, dado, byteOffset);
            if (dado == NULL)
            {
                mensagem_erro();
                return;
            }

            if (dado_get_removido(dado) == '0') // Verifica se o dado não foi removido
            {
                dado_imprimir(header, dado); // Imprime o dado se a busca for bem-sucedida
                printf("\n");
            }
            dado_apagar(&dado);
        }
    }
}

// 7 teste.bin arv.bin