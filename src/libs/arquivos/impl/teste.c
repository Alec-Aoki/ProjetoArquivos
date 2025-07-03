#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../registros/header.h"
#include "../../registros/dados.h"
#include "../../auxiliar/auxiliar.h"
#include "../../registros/busca.h"
#include "../arqBIN.h"
#include "../arvB.h"

int main()
{
    FILE *pontArqBin = fopen("arvoreB.bin", "rb+");
    if (pontArqBin == NULL)
    {
        mensagem_erro();
        return 1;
    }

    HEADER_ARVB *headerArq = NULL;
    headerArq = ArvB_header_ler(pontArqBin, headerArq);
    int BOraiz = ArvB_header_get_int(headerArq, 1);

    NO *no_raiz = NULL;
    no_raiz = ArvB_no_ler(pontArqBin, BOraiz);

    printf("ByteOffset da raiz: %d\n", ArvB_header_get_int(headerArq, 1));
    printf("Tipo do nó: %d\n", ArvB_no_get_int(no_raiz, 2));
    printf("Quantidade de chaves: %d\n", ArvB_no_get_int(no_raiz, 3));

    NO *no_encontrado = NULL;
    no_encontrado = ArvB_busca(pontArqBin, BOraiz, 12);
    if (no_encontrado != NULL)
    {
        printf("\nChave encontrada\n");
        printf("ByteOffset do dado: %d\n", ArvB_no_get_int(no_encontrado, 1));
        ArvB_no_apagar(&no_encontrado);
    }
    else
    {
        printf("Chave não encontrada.\n");
    }

    ArvB_header_apagar(&headerArq);
    ArvB_no_apagar(&no_raiz);
    fclose(pontArqBin);
    return 0;
}