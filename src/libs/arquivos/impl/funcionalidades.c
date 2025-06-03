#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../registros/header.h"
#include "../../registros/dados.h"
#include "../../registros/busca.h"
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
    char nomeArqBin[TAM_MAX_STR];
    ler_nome_arquivo(nomeArqBin);

    FILE *pontArqBin = fopen(nomeArqBin, "rb"); // Abrindo arquivo binário no modo de leitura
    if (pontArqBin == NULL)
    {
        mensagem_erro();
        return;
    }

    int quantBuscas;
    scanf("%d", &quantBuscas);

    BUSCA *busca = NULL;
    HEADER *headerArq = NULL;
    bool respostaEncontrada = false;

    headerArq = header_ler(pontArqBin, headerArq);

    for (int i = 0; i < quantBuscas; i++)
    {
        busca = busca_ler(busca); // Lendo parâmetros da busca

        if (!arqBIN_buscar_dado(pontArqBin, busca)) // Buscando dados no arquivo .bin
            mensagem_regInexistente();              // Não foi encontrado um dado que obedece os campos de busca

        printf("**********\n");

        busca_apagar(&busca);
    }

    header_apagar(&headerArq);
    fclose(pontArqBin);
    return;
}

/* funcionalidade4()
Remove logicamente um dado do arquivo binário
*/
void funcionalidade4()
{
    char nomeArqBin[TAM_MAX_STR];
    ler_nome_arquivo(nomeArqBin);

    FILE *pontArqBin = fopen(nomeArqBin, "rb+"); // Abrindo o arquivo binário no modo de leitura e escrita

    if (pontArqBin == NULL)
    {
        mensagem_erro();
        return;
    }

    int quantBuscas;
    scanf("%d", &quantBuscas);

    BUSCA *busca = NULL;
    HEADER *headerArq = NULL;

    headerArq = header_ler(pontArqBin, headerArq);
    
    for (int i = 0; i < quantBuscas; i++)
    {
        busca = busca_ler(busca); // Lendo parâmetros da busca

        if (!arqBIN_delete_dado(pontArqBin, busca))
        {
            mensagem_regInexistente();
        }

        busca_apagar(&busca);
    }

    header_apagar(&headerArq);
    fclose(pontArqBin);
    return;
}

/* funcionalidade5()
Insere um dado no arquivo binário
*/
void funcionalidade5()
{
    char nomeArqBin[TAM_MAX_STR];
    ler_nome_arquivo(nomeArqBin);

    int quantDados;
    scanf("%d", &quantDados);

    FILE *pontArqBin = fopen(nomeArqBin, "rb+"); // Abrindo o arquivo binário no modo de leitura e escrita

    if (pontArqBin == NULL)
    {
        mensagem_erro();
        return;
    }

    DADO *dado = NULL;
    HEADER *headerArq = NULL;
    headerArq = header_ler(pontArqBin, headerArq);
    if (headerArq == NULL)
    {
        mensagem_erro();
        fclose(pontArqBin);
        return;
    }

    int quantRegArq = header_get_nroRegArq(headerArq);
    int quantRegRem = header_get_nroRegRem(headerArq);

    /* Tentarei transferir para uma função no arqBIN*/
    for(int i = 0; i < quantDados; i++)
    {
        char **entrada = ler_entrada_insert(); // Lê a entrada do usuário

        if (entrada == NULL)
        {
            mensagem_erro();
            fclose(pontArqBin);
            return;
        }

        dado = dado_set(dado, 0, 0, -1, str_to_int(entrada[0]), str_to_int(entrada[1]),
                        str_to_float(entrada[2]), entrada[3], entrada[4], entrada[5], entrada[6]);

        int tamReg = dado_get_tamReg(dado);

        long int topo = header_get_topo(headerArq);

        if (topo == -1 )
        {
            // Se o topo for -1, significa que não há regitros logicamente removidos, logo insere-se no final do arquivo
            fseek(pontArqBin, 0, SEEK_END); // Posiciona o ponteiro no final do arquivo
            arqBIN_escrever_dado(pontArqBin, dado); // Escreve o dado no arquivo binário
            quantRegArq++;
        }
        else 
        {
            // Se o topo não for -1, significa que há registros logicamente removidos
            long int byteOffset = topo; // O byteOffset do registro a ser inserido é o topo
            while (byteOffset != -1) // Percorre a lista de registros removidos
            {
                dado = dado_ler(pontArqBin, dado, byteOffset); // Lê o dado no byteOffset atual

                if (dado_get_removido(dado) == '1')
                {
                    if (tamReg <= dado_get_tamReg(dado)) // Verifica se o tamanho do registro a ser inserido cabe no espaço do registro removido, estratégia First Fit
                    {
                        int numLixo = dado_get_tamReg(dado) - tamReg; // Calcula o número de bytes de lixo que sobrará após a inserção
                        // Inserir o dado no arquivo binário e preencher o espaço restante com lixo($)

                        quantRegArq++;
                        quantRegRem--;
                    }
                    else 
                    {
                        // Se o tamanho do registro a ser inserido não couber, continua para o próximo registro removido
                        byteOffset = dado_get_prox(dado); // Atualiza o byteOffset para o próximo registro removido
                    }
                }
            }
        }
        
    }
}