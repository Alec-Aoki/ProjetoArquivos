#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "./libs/arquivos/funcionalidades.h"

#include "./libs/arquivos/arvB.h"

/*
MAIN DE TESTE DA ARV B
*/

void print_header(HEADER_ARVB *header)
{
    if (header == NULL)
    {
        printf("Erro\n");
        return;
    }

    printf("Byteoffset raiz: %d\n", ArvB_header_get_int(header, 1));
    printf("RRN: %d\n", ArvB_header_get_int(header, 2));
    printf("Nro nos: %d\n\n", ArvB_header_get_int(header, 3));

    return;
}

int main(void)
{
    FILE *pontArq = fopen("teste.bin", "wb+");
    if (pontArq == NULL)
    {
        printf("Erro ao abrir arquivo\n");
    }

    HEADER_ARVB *header = ArvB_header_criar();
    print_header(header);

    ArvB_inserir(pontArq, header, 20, 200);
    header = ArvB_header_ler(pontArq, header);
    print_header(header);
    NO *no1 = ArvB_no_ler(pontArq, TAM_HEADER_ARVB);
    print_no(no1);

    ArvB_inserir(pontArq, header, 30, 300);
    header = ArvB_header_ler(pontArq, header);
    print_header(header);
    no1 = ArvB_no_ler(pontArq, TAM_HEADER_ARVB);
    print_no(no1);

    ArvB_inserir(pontArq, header, 10, 100);
    header = ArvB_header_ler(pontArq, header);
    print_header(header);
    no1 = ArvB_no_ler(pontArq, TAM_HEADER_ARVB);
    print_no(no1);
    NO *no2 = ArvB_no_ler(pontArq, TAM_HEADER_ARVB + TAM_REGISTRO_ARVB);
    print_no(no2);

    return 0;
}