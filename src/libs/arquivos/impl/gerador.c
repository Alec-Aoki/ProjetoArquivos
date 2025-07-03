#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
================================================================================
||                          BASE DE CÓDIGO FORNECIDA                         ||
||      (Colei aqui as suas funções para este exemplo ser compilável)       ||
================================================================================
*/

// Supondo que essas constantes/typedefs estejam em um .h
#define quantMaxChaves 2
#define quantMaxFilhos (quantMaxChaves + 1)
#define mensagem_erro() fprintf(stderr, "Ocorreu um erro.\n")

typedef struct header_arvB_ HEADER_ARVB;
typedef struct no_ NO;

struct header_arvB_
{
    char status;
    int noRaiz;
    int proxRRN;
    int nroNos;
};

struct no_
{
    int byteOffset;
    int chaves[quantMaxChaves];
    int byteOffsetDados[quantMaxChaves];
    int byteOffsetDescendentes[quantMaxFilhos];
    int tipoNo;
    int quantChavesAtual;
};

// Funções de Header
HEADER_ARVB *ArvB_header_criar()
{
    HEADER_ARVB *h = (HEADER_ARVB *)malloc(sizeof(HEADER_ARVB));
    if (h)
    {
        h->status = '1';
        h->noRaiz = -1;
        h->proxRRN = 0;
        h->nroNos = 0;
    }
    return h;
}

void ArvB_header_apagar(HEADER_ARVB **h)
{
    if (h && *h)
    {
        free(*h);
        *h = NULL;
    }
}

void ArvB_header_escrever(FILE *f, HEADER_ARVB *h)
{
    if (!f || !h)
        return;
    fseek(f, 0, SEEK_SET);
    fwrite(&h->status, sizeof(char), 1, f);
    fwrite(&h->noRaiz, sizeof(int), 1, f);
    fwrite(&h->proxRRN, sizeof(int), 1, f);
    fwrite(&h->nroNos, sizeof(int), 1, f);
}

// Funções de Nó
NO *ArvB_no_criar()
{
    NO *n = (NO *)malloc(sizeof(NO));
    if (n)
    {
        n->byteOffset = -1;
        for (int i = 0; i < quantMaxChaves; i++)
        {
            n->chaves[i] = -1;
            n->byteOffsetDados[i] = -1;
        }
        for (int i = 0; i < quantMaxFilhos; i++)
        {
            n->byteOffsetDescendentes[i] = -1;
        }
        n->tipoNo = -1;
        n->quantChavesAtual = 0;
    }
    return n;
}

void ArvB_no_apagar(NO **n)
{
    if (n && *n)
    {
        free(*n);
        *n = NULL;
    }
}

void ArvB_no_escrever(FILE *f, NO *n)
{
    if (!f || !n || n->byteOffset < 0)
        return;
    fseek(f, n->byteOffset, SEEK_SET);
    fwrite(&n->byteOffset, sizeof(int), 1, f);
    fwrite(n->chaves, sizeof(int), quantMaxChaves, f);
    fwrite(n->byteOffsetDados, sizeof(int), quantMaxChaves, f);
    fwrite(n->byteOffsetDescendentes, sizeof(int), quantMaxFilhos, f);
    fwrite(&n->tipoNo, sizeof(int), 1, f);
    fwrite(&n->quantChavesAtual, sizeof(int), 1, f);
}

/*
================================================================================
||                   PROGRAMA GERADOR DO ARQUIVO DE TESTE                     ||
================================================================================
*/

int main()
{
    printf("Iniciando a criacao do arquivo de teste 'arvoreB.bin'...\n");

    FILE *arq = fopen("arvoreB.bin", "wb");
    if (arq == NULL)
    {
        perror("Nao foi possivel criar o arquivo");
        return 1;
    }

    // 1. Criar e escrever o header
    HEADER_ARVB *header = ArvB_header_criar();
    header->status = '1';
    header->noRaiz = 13;   // Raiz começa após o header (que tem 13 bytes)
    header->proxRRN = 173; // Próximo offset livre (13 + 4 nós * 40 bytes/nó)
    header->nroNos = 4;    // 1 raiz + 3 folhas
    ArvB_header_escrever(arq, header);
    ArvB_header_apagar(&header);
    printf("-> Header escrito no arquivo.\n");

    // 2. Criar e escrever o Nó Raiz (offset 13)
    NO *no_raiz = ArvB_no_criar();
    no_raiz->byteOffset = 13;
    no_raiz->tipoNo = 0; // 0=raiz
    no_raiz->quantChavesAtual = 2;
    no_raiz->chaves[0] = 10;
    no_raiz->chaves[1] = 20;
    no_raiz->byteOffsetDados[0] = 100; // Dados fictícios
    no_raiz->byteOffsetDados[1] = 200;
    no_raiz->byteOffsetDescendentes[0] = 53;  // Aponta para o nó com chave 5
    no_raiz->byteOffsetDescendentes[1] = 93;  // Aponta para o nó com chaves 12, 15
    no_raiz->byteOffsetDescendentes[2] = 133; // Aponta para o nó com chaves 25, 30
    ArvB_no_escrever(arq, no_raiz);
    ArvB_no_apagar(&no_raiz);
    printf("-> No Raiz (chaves 10, 20) escrito no arquivo.\n");

    // 3. Criar e escrever a Folha 1 (offset 53)
    NO *folha1 = ArvB_no_criar();
    folha1->byteOffset = 53;
    folha1->tipoNo = -1; // -1=folha
    folha1->quantChavesAtual = 1;
    folha1->chaves[0] = 5;
    folha1->byteOffsetDados[0] = 50;
    ArvB_no_escrever(arq, folha1);
    ArvB_no_apagar(&folha1);
    printf("-> No Folha (chave 5) escrito no arquivo.\n");

    // 4. Criar e escrever a Folha 2 (offset 93)
    NO *folha2 = ArvB_no_criar();
    folha2->byteOffset = 93;
    folha2->tipoNo = -1;
    folha2->quantChavesAtual = 2;
    folha2->chaves[0] = 12;
    folha2->chaves[1] = 15;
    folha2->byteOffsetDados[0] = 120;
    folha2->byteOffsetDados[1] = 150;
    ArvB_no_escrever(arq, folha2);
    ArvB_no_apagar(&folha2);
    printf("-> No Folha (chaves 12, 15) escrito no arquivo.\n");

    // 5. Criar e escrever a Folha 3 (offset 133)
    NO *folha3 = ArvB_no_criar();
    folha3->byteOffset = 133;
    folha3->tipoNo = -1;
    folha3->quantChavesAtual = 2;
    folha3->chaves[0] = 25;
    folha3->chaves[1] = 30;
    folha3->byteOffsetDados[0] = 250;
    folha3->byteOffsetDados[1] = 300;
    ArvB_no_escrever(arq, folha3);
    ArvB_no_apagar(&folha3);
    printf("-> No Folha (chaves 25, 30) escrito no arquivo.\n");

    fclose(arq);
    printf("\nArquivo 'arvoreB.bin' gerado com sucesso e pronto para os testes!\n");

    return 0;
}