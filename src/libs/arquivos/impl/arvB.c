#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*/#include "../../registros/header.h"
#include "../../registros/dados.h"
#include "../../auxiliar/auxiliar.h"
#include "../../registros/busca.h"*/
#include "../../auxiliar/auxiliar.h"

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

typedef struct promocao_
{
    NO *noNovo;
    int chavePromov;
    int byteOffsetDadoPromov;
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

/* ArvB_no_get_int():
Retorna o valor de um campo inteiro de uma struct do tipo no.
Parâmetros: ponteiro para a struct e o campo desejado
    1: byteOffset
    2: tipoNO
    3: quantChavesAtual
Retorna: valor do campo (-1 se não encontrado ou no nulo)
*/
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

// Funções internas auxiliares da inserção:
int inserir_ordenado(int *chaves, int *byteOffsetDados, int *byteOffsetDescendentes,
                     int *quantChavesAtual, int chave, int byteOffsetDado, int byteOffsetFilho);
PROMOCAO ArvB_inserir_recursivo(FILE *pontArq, HEADER_ARVB *header, int chave, int byteOffsetDado, int byteOffsetNoAtual);
PROMOCAO ArvB_split(NO *no, HEADER_ARVB *header, int chave, int byteOffsetDado, int byteOffsetFilho);

/* ArvB_inserir():
Função "macro" para mexer no header e lidar com a primeira inserção
Parâmetros: ponteiro para arquivo, ponteiro para header de arvB, chave a ser inserido e seus campos
*/
void ArvB_inserir(FILE *pontArq, HEADER_ARVB *header, int chave, int byteOffsetDado)
{
    if (pontArq == NULL || header == NULL)
        return; // Erro

    header->status = '0';
    ArvB_header_escrever(pontArq, header);

    int byteOffsetNoRaiz = header->noRaiz;

    // Caso 1: árvore vázia
    if (byteOffsetNoRaiz == -1)
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
            noRaiz->byteOffsetDescendentes[i] = -1;
        }

        // Definindo campos do header
        header->noRaiz = noRaiz->byteOffset;
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
            raizNova->byteOffsetDescendentes[0] = byteOffsetNoRaiz;
            raizNova->byteOffsetDescendentes[1] = promocao.noNovo->byteOffset;

            // Inicializando os campos restantes
            for (int i = 1; i < quantMaxChaves; i++)
            {
                raizNova->chaves[i] = -1;
                raizNova->byteOffsetDados[i] = -1;
            }
            for (int i = 2; i < quantMaxFilhos; i++)
                raizNova->byteOffsetDescendentes[i] = -1;

            // Atualizando os tipos dos nós filhos
            NO *noRaizAntigo = ArvB_no_ler(pontArq, byteOffsetNoRaiz);
            if (noRaizAntigo != NULL)
            {
                int tipoNo = -1; // Folha
                for (int i = 0; i < quantMaxFilhos; i++)
                {
                    if (noRaizAntigo->byteOffsetDescendentes[i] != -1)
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
                if (promocao.noNovo->byteOffsetDescendentes[i] != -1)
                {
                    tipoNo = 1; // Intermediario
                    break;
                }
            }
            promocao.noNovo->tipoNo = tipoNo;
            ArvB_no_escrever(pontArq, promocao.noNovo);

            ArvB_no_escrever(pontArq, raizNova); // Escrevendo raíz nova

            // Atualizando header
            header->noRaiz = raizNova->byteOffset;
            header->proxRRN++;
            header->nroNos++;

            ArvB_no_apagar(&raizNova);
            ArvB_no_apagar(&(promocao.noNovo));
        }
    }

    // Definindo header como consistente e escrevendo
    header->status = '1';
    ArvB_header_escrever(pontArq, header);
    return;
}

/* ArvB_inserir_recursivo():
Caso "geral" da inserção, que busca pela nó em que a chave será inserida usando recursão
Parâmetros: ponteiro para arquivo, ponteiro para header de arvB, chave a ser inserido e seus campos
Retorna: struct da chave que foi promovida (se houve split)
*/
PROMOCAO ArvB_inserir_recursivo(FILE *pontArq, HEADER_ARVB *header, int chave, int byteOffsetDado, int byteOffsetNoAtual)
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
        PROMOCAO promocaoFilho = ArvB_inserir_recursivo(pontArq, header, chave, byteOffsetDado, noAtual->byteOffsetDescendentes[pos]);

        // Caso 1: houve split e promoção
        if (promocaoFilho.houveSplit)
        {
            // Caso 1: o nó atual tem espaço pra chave que foi promovida
            if (noAtual->quantChavesAtual < quantMaxChaves)
            {
                inserir_ordenado(noAtual->chaves, noAtual->byteOffsetDados, noAtual->byteOffsetDescendentes, &(noAtual->quantChavesAtual),
                                 promocaoFilho.chavePromov, promocaoFilho.byteOffsetDadoPromov, ArvB_no_get_int(promocaoFilho.noNovo, 1));

                ArvB_no_escrever(pontArq, noAtual);

                ArvB_no_apagar(&(promocaoFilho.noNovo));

                promocao.houveSplit = false;
            }
            else // Caso 2: não tem espaço, split
            {
                promocao = ArvB_split(noAtual, header, promocaoFilho.chavePromov, promocaoFilho.byteOffsetDadoPromov,
                                      ArvB_no_get_int(promocaoFilho.noNovo, 1));

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
PROMOCAO ArvB_split(NO *no, HEADER_ARVB *header, int chave, int byteOffsetDado, int byteOffsetFilho)
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
    int byteOffsetDadosTemp[quantMaxChaves + 1];
    int byteOffsetFilhosTemp[quantMaxFilhos + 1];
    int quantChavesTemp = quantMaxChaves;

    // Inicializando vetores temporários
    for (int i = 0; i < quantMaxChaves + 1; i++)
    {
        chavesTemp[i] = -1;
        byteOffsetDadosTemp[i] = -1;
    }
    for (int i = 0; i < quantMaxFilhos + 1; i++)
        byteOffsetFilhosTemp[i] = -1;

    // Copiando dados do nó para estes vetores
    for (int i = 0; i < quantMaxChaves; i++)
    {
        chavesTemp[i] = no->chaves[i];
        byteOffsetDadosTemp[i] = no->byteOffsetDados[i];
    }
    for (int i = 0; i < quantMaxFilhos; i++)
        byteOffsetFilhosTemp[i] = no->byteOffsetDescendentes[i];

    // Inserindo a chave nos vetores temporários
    inserir_ordenado(chavesTemp, byteOffsetDadosTemp, byteOffsetFilhosTemp, &quantChavesTemp, chave, byteOffsetDado, byteOffsetFilho);

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
        no->byteOffsetDescendentes[i] = -1;

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
        no->byteOffsetDescendentes[i] = byteOffsetFilhosTemp[i];

    // Nó direito
    j = 0;
    for (int i = meio + 1; i < quantMaxFilhos + 1; i++)
    {
        noNovo->byteOffsetDescendentes[j] = byteOffsetFilhosTemp[i];
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
int inserir_ordenado(int *chaves, int *byteOffsetDados, int *byteOffsetDescendentes,
                     int *quantChavesAtual, int chave, int byteOffsetDado, int byteOffsetFilho)
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

    if (byteOffsetFilho != -2 && byteOffsetDescendentes != NULL)
    {
        // Deslocando pra direita
        for (int i = quantMaxFilhos - 1; i > pos + 1; i--)
            byteOffsetDescendentes[i] = byteOffsetDescendentes[i - 1];

        byteOffsetDescendentes[pos + 1] = byteOffsetFilho;
    }

    return pos;
}

// Função para debuggar
void print_no(NO *no)
{
    if (no == NULL)
        return;

    printf("**********************\n");
    printf("Byteoffset: %d\n", no->byteOffset);
    printf("Tipo no: %d\n", no->tipoNo);
    printf("Chave 1: %d\n", no->chaves[0]);
    printf("Chave 2: %d\n\n", no->chaves[1]);
    /*printf("Byteoffset Chave 1: %d\n", no->byteOffsetDados[0]);
    printf("Byteoffset Chave 2: %d\n", no->byteOffsetDados[1]);
    printf("ByteOffset Filho 1: %d\n", no->byteOffsetDescendentes[0]);
    printf("ByteOffset Filho 2: %d\n", no->byteOffsetDescendentes[1]);
    printf("ByteOffset Filho 2: %d\n", no->byteOffsetDescendentes[2]);*/
    printf("**********************\n\n");

    return;
}