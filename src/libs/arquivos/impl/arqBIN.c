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

long int arqBIN_buscar_byteOffset(FILE *pontArqBIN, BUSCA *busca)
{
    if (pontArqBIN == NULL || busca == NULL)
    {
        mensagem_erro();
        return -1;
    }

    HEADER *headerArq = header_ler(pontArqBIN, headerArq);

    if (headerArq == NULL)
    {
        mensagem_erro();
        return -1;
    }

    int quantRegArq = header_get_nroRegArq(headerArq);

    long int byteOffset = BYTEOFFSET_HEADER; // Inicializado com o tamanho do header

    DADO *dado = NULL;
    long int byteOffsetEncontrado = -1; // Inicializado com -1 (não encontrado)
    
    // Loop para ler e verificar dados até que o arquivo ou a quantidade de respostas acabe
    while (quantRegArq > 0)
    {
        dado = dado_ler(pontArqBIN, dado, byteOffset); // Lendo dado do arquivo
        if (dado == NULL)
            break; //Erro
        
        if (busca_comparar(busca, dado))
        {
            byteOffsetEncontrado = byteOffset;
            break;
        }
        
        byteOffset += dado_get_tamReg(dado) + 5; // Atualizando o byteOffset (indo para a leitura do próximo dado)
        quantRegArq--; // Atualizando a quantidade de dados buscada
    }

    dado_apagar(&dado);
    header_apagar(&headerArq);

    return byteOffsetEncontrado;
}

bool arqBIN_delete_dado(FILE *pontArqBIN, BUSCA *busca)
{
    if (pontArqBIN == NULL || busca == NULL)
    {
        mensagem_erro();
        return false;
    }

    HEADER *headerArq = NULL;
    headerArq = header_ler(pontArqBIN, headerArq);

    if (headerArq == NULL)
    {
        mensagem_erro();
        return false;
    }

    long int topo = header_get_topo(headerArq);
    long int prox;
    int quantRegArq = header_get_nroRegArq(headerArq);
    int quantRegRem = header_get_nroRegRem(headerArq);

    // buscar registro que satisfaz os campos de busca
    long int byteOffset = arqBIN_buscar_byteOffset(pontArqBIN, busca);
    
    if (byteOffset == -1)
    {
        // registro não encontrado
        header_apagar(&headerArq);
        return false;
    }

    // guardar o byteOffset do registro encontrado
    DADO *dado = dado_ler(pontArqBIN, dado, byteOffset);

    if (dado == NULL)
    {
        header_apagar(&headerArq);
        return false;
    }

    // remover logicamente o dado
    dado_set_removido(dado, '1');
    quantRegArq--; // decrementa o número de registros no arquivo
    quantRegRem++; // incrementa o número de registros removidos
    
    if (topo == -1)
    {
        // se o topo for -1, significa que não há registros removidos
        topo = byteOffset; // define o topo como o byteOffset do registro removido
        dado_set_prox(dado, -1); // define o próximo como -1
        // header_set_proxByteOffset(headerArq, byteOffset); // atualiza o próximo byteOffset no header
    }
    else 
    {
        // se o topo não for -1, significa que há registros removidos
        dado_set_prox(dado, topo); // define o próximo como o topo atual
        topo = byteOffset; // atualiza o topo para o byteOffset do registro removido
    }

    // atualizar os campos do header e do dado
    header_set_topo(headerArq, topo);
    header_set_nroRegArq(headerArq, quantRegArq);
    header_set_nroRegRem(headerArq, quantRegRem);

    // escrever o dado e o header atualizados no arquivo
    fseek(pontArqBIN, byteOffset, SEEK_SET); // posiciona o ponteiro do arquivo no byteOffset do registro removido
    arqBIN_escrever_dado(pontArqBIN, dado); // escreve o dado atualizado no arquivo
    header_escrever(pontArqBIN, headerArq, false); // escreve o header atualizado no arquivo

    dado_apagar(&dado);
    header_apagar(&headerArq);
    fclose(pontArqBIN);
    return true; 
}
