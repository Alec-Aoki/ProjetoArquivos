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

    // Escrevendo header no arquivo binário
    header_escrever(pontArqBIN, header, camposSemanticos);

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

    // Escrevendo header no arquivo binário
    dado_escrever(pontArqBIN, dado, 0);

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

    // Leitura do header do arquivo
    HEADER *headerArq = header_ler(pontArqBIN, headerArq);
    if (headerArq == NULL)
    {
        mensagem_erro();
        return;
    }

    // ByteOffset inicializado com tamanho do header
    int byteOffset = BYTEOFFSET_HEADER;
    DADO *dadoTemp = NULL; // Struct reutilizável

    // Contador para loop de leitura de dados
    int quantRegArq = header_get_int(headerArq, 1);
    if (quantRegArq == 0)
    {
        mensagem_regInexistente();
        return;
    }

    // Loop de leitura de dados
    while (quantRegArq > 0)
    {
        // Lê dado do arq. bin
        dadoTemp = dado_ler(pontArqBIN, dadoTemp, byteOffset);
        if (dadoTemp == NULL)
        {
            mensagem_erro();
            return; // Erro ao ler o dado
        }
        // Imprimindo o dado somente se ele não estiver removido
        if (dado_get_removido(dadoTemp) != '1')
        {
            dado_imprimir(headerArq, dadoTemp);
            printf("\n");
        }

        // Atualizando valores
        byteOffset += dado_get_int(dadoTemp, 3) + 5;
        quantRegArq--;
    }

    // Apaga struct reutilizável e header
    dado_apagar(&dadoTemp);
    header_apagar(&headerArq);

    return;
}

/* arqBIN_buscar_byteOffset():
Busca um dado que satisfaz os campos num arquivo .bin
a partir do byteOffset passado. Caso o byteOffset não
seja válido, busca do começo do arquivo.
Parâmetro: ponteiro para arquivo, ponteiro para struct busca,
byteOffset do qual a busca deverá iniciar
Retorna: o byteOffset do dado encontrado ou
-1 caso não tenha sido encontrado no arq. inteiro
-2 caso não tenha sido encontrado a partir do byteOffset fornecido
-3 caso o byteOffset passado tenha sido maior que o arq.
*/
long int arqBIN_buscar_byteOffset(FILE *pontArqBIN, BUSCA *busca,
                                  HEADER *headerArq, long int byteOffset)
{
    if (pontArqBIN == NULL || busca == NULL)
    {
        mensagem_erro();
        return -1;
    }

    if (headerArq == NULL)
    {
        mensagem_erro();
        return -1;
    }

    bool buscaInicio = false; // Flag para mandar a mensagem de erro correta
    int quantRegArq = header_get_int(headerArq, 1);
    // Campo proxByteOffset do header
    long int byteOffsetFimArq = header_get_longint(headerArq, 2);
    if (byteOffset > byteOffsetFimArq)
        return -3; // Erro -3 (Tentou buscar após o fim do arquivo)

    // Caso não seja fornecido, começa do início do arquivo
    if (byteOffset < 0)
    {
        byteOffset = BYTEOFFSET_HEADER;
        buscaInicio = true;
    }
    DADO *dado = NULL; // Reutilizável

    // Loop para ler e verificar dados até que o arquivo ou a quantidade de respostas acabe
    while ((quantRegArq > 0) && (byteOffset < byteOffsetFimArq))
    {
        dado = dado_ler(pontArqBIN, dado, byteOffset); // Lendo dado do arquivo
        if (dado == NULL)
        {
            break;
        }

        // Pula o dado se ele tiver sido removido
        if (dado_get_removido(dado) == '0')
        {
            // Vê se o dado satisfaz os parâmetros da busca
            if (busca_comparar(busca, dado))
                return byteOffset; // Se sim, retorna byteOffset

            // Atualizando a quantidade de dados buscada
            quantRegArq--;
        }

        // Atualizando o byteOffset (indo para a leitura do próximo dado)
        byteOffset += dado_get_int(dado, 3) + 5;
    }

    if (buscaInicio)
        return -1; // Erro -1 (Não encontrou no arquivo inteiro)
                   // Else
    return -2;     // Erro -2 (Não encontrou a partir do byteOffset fornecido)
}

/* arqBIN_delete_dado():
Remove logicamente um dado de um arquivo .bin
Parâmetro: ponteiro para arquivo, ponteiro para struct busca
Retorna: booleano (true se removido, falso senão)
*/
bool arqBIN_delete_dado(FILE *pontArqBIN, BUSCA *busca, HEADER *headerArq)
{
    if (pontArqBIN == NULL || busca == NULL)
    {
        mensagem_erro();
        return false;
    }

    if (headerArq == NULL)
    {
        mensagem_erro();
        return false;
    }

    long int topo = header_get_longint(headerArq, 1);
    long int prox = -1;
    int quantRegArq = header_get_int(headerArq, 1);
    int quantRegRem = header_get_int(headerArq, 2);

    // Buscar registro que satisfaz os campos de busca
    long int byteOffset = arqBIN_buscar_byteOffset(pontArqBIN, busca, headerArq, -1);

    if (byteOffset == -1)
    {
        // Registro não encontrado
        // Header_apagar(&headerArq);
        return false;
    }

    // Guardar o byteOffset do registro encontrado
    DADO *dado = NULL;
    dado = dado_ler(pontArqBIN, dado, byteOffset);
    if (dado == NULL)
    {
        // Header_apagar(&headerArq);
        return false;
    }

    // Remover logicamente o dado
    dado = dado_set(dado, 1, -2, -2, -2, -2, -2,
                    NULL, NULL, NULL, NULL);
    quantRegArq--; // Decrementa o número de registros no arquivo
    quantRegRem++; // Incrementa o número de registros removidos

    if (topo == -1)
    {
        // se o topo for -1, significa que não há registros removidos
        topo = byteOffset; // define o topo como o byteOffset do registro removido
        dado = dado_set(dado, -2, -2, -1, -2, -2, -2,
                        NULL, NULL, NULL, NULL);
    }
    else
    {
        // se o topo não for -1, significa que há registros removidos
        dado = dado_set(dado, -2, -2, topo, -2, -2, -2,
                        NULL, NULL, NULL, NULL);
        topo = byteOffset; // atualiza o topo para o byteOffset do registro removido
    }

    // Atualiza os campos do header e do dado
    headerArq = header_set(headerArq, -2, topo, -2, quantRegArq, quantRegRem,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // escrever o dado e o header atualizados no arquivo
    fseek(pontArqBIN, byteOffset, SEEK_SET); // posiciona o ponteiro do arquivo no byteOffset do registro removido
    arqBIN_escrever_dado(pontArqBIN, dado);  // escreve o dado atualizado no arquivo

    dado_apagar(&dado);

    return true;
}

/* arqBIN_insert_dado():
Insere um dado no arquivo binário, utilizando estratégia de inserção First Fit.
Parâmetro: ponteiro para arquivo, ponteiro para array de strings (entrada)
Retorna: booleano (true se inserido, false se erro)
*/
bool arqBIN_insert_dado(FILE *pontArqBIN, char **entrada)
{
    if (entrada == NULL)
    {
        mensagem_erro();
        fclose(pontArqBIN);
        return false;
    }

    HEADER *headerArq = NULL;
    headerArq = header_ler(pontArqBIN, headerArq);

    DADO *dado = NULL;
    dado = dado_set(dado, 0, 0, -1, str_to_int(entrada[0]),
                    str_to_int(entrada[1]), str_to_float(entrada[2]),
                    entrada[3], entrada[4], entrada[5], entrada[6]);

    // Inicializando variáveis necessárias
    int tamReg = dado_get_int(dado, 3);
    int nroRegArq = header_get_int(headerArq, 1);
    int nroRegRem = header_get_int(headerArq, 2);
    long int topo = header_get_longint(headerArq, 1);
    bool inserido = false;

    if (topo == -1)
    {
        // Se o topo for -1, significa que não há registros logicamente removidos,
        // logo insere-se no final do arquivo
        long int proxByteOffset = header_get_longint(headerArq, 2);
        fseek(pontArqBIN, proxByteOffset, SEEK_SET);
        arqBIN_escrever_dado(pontArqBIN, dado);

        proxByteOffset += tamReg + 5; // Atualiza o próximo byte offset
        nroRegArq++;
        inserido = true;
        // Atualiza o próximo byte offset no header
        headerArq = header_set(headerArq, -2, -2, proxByteOffset, nroRegArq, -2,
                               NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    }
    else // Há registro logic. rem.
    {
        long int byteOffset = topo;
        DADO *dadoRem = NULL;

        while (byteOffset != -1)
        {
            dadoRem = dado_ler(pontArqBIN, dado, byteOffset);

            if (dado_get_removido(dadoRem) == '1')
            {
                if (tamReg <= dado_get_int(dadoRem, 3))
                {
                    // Se o tamanho do registro couber no registro removido,
                    // insere o dado e preenche com lixo($)
                    int nroLixo = dado_get_int(dadoRem, 3) - tamReg;
                    // dado_set_tamReg(dado, dado_get_int(dadoRem, 3));
                    dado = dado_set(dado, -2, dado_get_int(dadoRem, 3), -2, -2, -2, -2,
                                    NULL, NULL, NULL, NULL);

                    // inserir o dado e preencher com lixo
                    dado_escrever(pontArqBIN, dado, nroLixo);

                    // Define o dado como não removido
                    // E define o próximo do dado como o próximo do dado removido
                    dado = dado_set(dado, 0, -2, dado_get_prox(dadoRem), -2, -2, -2,
                                    NULL, NULL, NULL, NULL);

                    // Atualiza o topo para o próximo do dado removido
                    topo = dado_get_prox(dadoRem);

                    // Atualiza o número de registros
                    nroRegArq++;
                    nroRegRem--;
                    inserido = true; // marca que o dado foi inserido
                    break;
                }
            }

            // Se o tamanho do registro não couber, continua para o próximo registro removido
            byteOffset = dado_get_prox(dadoRem);
            dado_apagar(&dadoRem);
        }
    }

    if (!inserido)
    {
        // Se não foi inserido, inserir no final do arquivo
        long int proxByteOffset = header_get_longint(headerArq, 2);
        fseek(pontArqBIN, proxByteOffset, SEEK_SET); // trocar por proxByteOffset
        arqBIN_escrever_dado(pontArqBIN, dado);

        proxByteOffset += tamReg + 5; // Atualiza o próximo byte offset
        // Atualiza o próximo byte offset no header
        headerArq = header_set(headerArq, -2, -2, proxByteOffset, -2, -2,
                               NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        nroRegArq++;
        inserido = true;
    }

    // Atualizando o header
    headerArq = header_set(headerArq, -2, topo, -2, nroRegArq, nroRegRem,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    fseek(pontArqBIN, 0, SEEK_SET);
    header_escrever(pontArqBIN, headerArq, false); // Escrevendo o header atualizado no arquivo

    dado_apagar(&dado);
    header_apagar(&headerArq);
    return inserido; // Retorna true se o dado foi inserido, false caso contrário
}