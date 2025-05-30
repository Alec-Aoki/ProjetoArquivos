#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../registros/header.h"
#include "../../registros/dados.h"
#include "../../auxiliar/auxiliar.h"
#include "../arqBIN.h"

/*arqBin_escrever_header():
Escreve uma struct do tipo header em um arquivo.bin
Parâmetro: ponteiro para arquivo, ponteiro para header, valor booleano
*/
void arqBIN_escrever_header(FILE *pontArqBIN, HEADER *header, bool camposSemanticos)
{
    if (pontArqBIN == NULL || header == NULL)
    {
        mensagem_erro();
        return;
    }

    header_escrever(pontArqBIN, header, camposSemanticos); // Escrevendo header no arquivo binário

    return;
}

/*arqBIN_escrever_dado():
Lê um dado de um arquivo .bin
Parâmetro: ponteiro para arquivo, ponteiro para dado
*/
void arqBIN_escrever_dado(FILE *pontArqBIN, DADO *dado)
{
    if (pontArqBIN == NULL || dado == NULL)
    {
        mensagem_erro();
        return;
    }

    dado_escrever(pontArqBIN, dado); // Escrevendo header no arquivo binário

    return;
}

/*arqBIN_imprimir():
Imprime um arquivo.bin
Parâmetro: ponteiro para arquivo
*/
void arqBIN_imprimir(FILE *pontArqBIN)
{
    if (pontArqBIN == NULL)
    {
        mensagem_erro();
        return;
    }

    HEADER *headerArq = header_ler(pontArqBIN, headerArq); // Leitura do header do arquivo
    if (headerArq == NULL)
    {
        mensagem_erro();
        return;
    }

    int byteOffset = BYTEOFFSET_HEADER; // Inicializado com tamanho do header
    DADO *dadoTemp = NULL;

    int quantRegArq = header_get_nroRegArq(headerArq); // Contador para loop de leitura de dados
    if (quantRegArq == 0)
    {
        mensagem_regInexistente();
        return;
    }

    // Loop de leitura de dados
    while (quantRegArq > 0)
    {
        dadoTemp = dado_ler(pontArqBIN, dadoTemp, byteOffset);
        // Imprimindo o dado somente se ele não estiver removido
        if (dado_get_removido(dadoTemp) != '1')
        {
            dado_imprimir(headerArq, dadoTemp);
            printf("\n");
        }

        byteOffset += dado_get_tamReg(dadoTemp) + 5;
        quantRegArq--;
    }

    dado_apagar(&dadoTemp);
    header_apagar(&headerArq);

    return;
}