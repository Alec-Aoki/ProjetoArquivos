#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../registros/header.h"
#include "../../registros/dados.h"
#include "../../auxiliar/auxiliar.h"
#include "../arqCSV.h"
#include "../arqBIN.h"
#include "../funcionalidades.h"

/* funcionalidade1()
Lê um arquivo.csv e o transforma em .bin
*/
void funcionalidade1()
{
    char nomeArqCSV[TAM_MAX_STR];
    char nomeArqBin[TAM_MAX_STR];

    ler_nome_arquivo(nomeArqCSV);
    ler_nome_arquivo(nomeArqBin);

    FILE *pontArqCSV = fopen(nomeArqCSV, "r");
    if (pontArqCSV == NULL)
    {
        mensagem_erro();
        return;
    }
    FILE *pontArqBin = fopen(nomeArqBin, "wb");
    if (pontArqBin == NULL)
    {
        mensagem_erro();
        return;
    }

    // Lendo header
    HEADER *headerCSV = arqCSV_ler_header(pontArqCSV);
    if (headerCSV == NULL)
    {
        mensagem_erro();
        return;
    }

    // Escrevendo o header  no arquivo binário
    fseek(pontArqBin, 0, SEEK_SET);
    header_set_status(headerCSV, '0'); // Inconsistente
    arqBIN_escrever_header(pontArqBin, headerCSV, true);

    // Lendo dados do .csv e escrevendo no .bin
    DADO *dadoCSV = NULL;                            // Ponteiro para dado (reutilizável)
    int quantRegDados = 0;                           // Contador de dados
    long int byteOffsetPonteiro = BYTEOFFSET_HEADER; // Inicializado com o tamanho do header
    fseek(pontArqCSV, 1, SEEK_CUR);                  // Avança para a próxima linha (1º dado)

    while (true)
    {
        dadoCSV = arqCSV_ler_dado(pontArqCSV);
        if (dadoCSV == NULL)
            break;

        arqBIN_escrever_dado(pontArqBin, dadoCSV);
        quantRegDados++;
        byteOffsetPonteiro += dado_get_tamReg(dadoCSV) + 5;

        dado_apagar(&dadoCSV);
    }
    // Atualizando campos do header
    header_set_nroRegArq(headerCSV, quantRegDados);
    header_set_proxByteOffset(headerCSV, byteOffsetPonteiro);
    header_set_status(headerCSV, '1'); // Consistente

    // Escrevendo o header no arquivo binário
    fseek(pontArqBin, 0, SEEK_SET);
    arqBIN_escrever_header(pontArqBin, headerCSV, false);

    header_apagar(&headerCSV);
    fclose(pontArqCSV);
    fclose(pontArqBin);

    binarioNaTela(nomeArqBin);

    return;
}

/* funcionalidade2()
Imprime todo o conteúdo de um arquivo binário
*/
void funcionalidade2()
{
    char nomeArqBin[TAM_MAX_STR];
    ler_nome_arquivo(nomeArqBin);

    FILE *pontArqBin = fopen(nomeArqBin, "rb"); // Abrindo arquivo binário no modo de leitura
    if (pontArqBin == NULL)
    {
        mensagem_erro();
        return;
    }

    fseek(pontArqBin, 0, SEEK_SET); // Posiciona o ponteiro no início do arquivo

    arqBIN_imprimir(pontArqBin);

    fclose(pontArqBin);
    return;
}

/* funcionalidade3()
Imprime na tela a quantidade requisitada de dados que satisfazer os campos de busca
*/
void funcionalidade3()
{
}
