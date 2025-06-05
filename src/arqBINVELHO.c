#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../registros/header.h"
#include "../../registros/dados.h"
#include "../../auxiliar/auxiliar.h"
#include "../../registros/busca.h"
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

/*arqBIN_buscar_dado():
Faz uma busca num arquivo .bin de acordo com a struct busca passada
Parâmetro: ponteiro para arquivo, ponteiro para struct busca
Retorna: booleano (true se encontrado, falso senão)
*/
bool arqBIN_buscar_dado(FILE *pontArqBIN, BUSCA *busca)
{
    if (pontArqBIN == NULL || busca == NULL)
    {
        mensagem_erro();
        return false;
    }

    /*Recuperando e imprimindo os dados*/
    // Preparando para leitura de dados
    HEADER *headerArq = NULL;
    headerArq = header_ler(pontArqBIN, headerArq);

    int quantRegArq = header_get_nroRegArq(headerArq);

    int byteOffset = BYTEOFFSET_HEADER;

    DADO *dado = NULL;
    bool respostaEncontrada = false;

    // Loop para ler e verificar dados até que o arquivo ou a quantidade de respostas acabe
    while (quantRegArq > 0)
    {
        dado = dado_ler(pontArqBIN, dado, byteOffset); // Lendo dado do arquivo
        if (dado == NULL)
            break; // Erro

        if (busca_comparar(busca, dado))
        {
            respostaEncontrada = true;
            dado_imprimir(headerArq, dado);
            printf("\n");
        }

        byteOffset += dado_get_tamReg(dado) + 5; // Atualizando o byteOffset (indo para a leitura do próximo dado)
        quantRegArq--;                           // Atualizando a quantidade de dados buscada
    }

    dado_apagar(&dado);
    header_apagar(&headerArq);

    return respostaEncontrada;
}