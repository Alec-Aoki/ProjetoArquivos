#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./libs/arquivos/funcionalidades.h"

#include "./libs/arquivos/arvB.h"

/*
MAIN DE TESTE DA ARV B
*/
int main(void)
{

    FILE *pontArq = fopen("teste.bin", "wb+");
    if (pontArq == NULL)
    {
        printf("Erro ao abrir arquivo\n");
    }

    HEADER_ARVB *header = ArvB_header_criar();
    printf("Header antes\n");
    printf("Byteoffset raiz: %d\n", ArvB_header_get_int(header, 1));
    printf("RRN: %d\n", ArvB_header_get_int(header, 2));
    printf("Nro nos: %d\n\n", ArvB_header_get_int(header, 3));
    ArvB_header_escrever(pontArq, header);

    ArvB_inserir(pontArq, header, 20, 200);

    header = ArvB_header_ler(pontArq, header);
    printf("Header depois\n");
    printf("Byteoffset raiz: %d\n", ArvB_header_get_int(header, 1));
    printf("RRN: %d\n", ArvB_header_get_int(header, 2));
    printf("Nro nos: %d\n\n", ArvB_header_get_int(header, 3));

    return 0;
}