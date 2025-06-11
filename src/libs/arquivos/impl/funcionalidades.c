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

    /*Abrindo arquivos CSV e BIN*/
    // Modo de leitura
    FILE *pontArqCSV = fopen(nomeArqCSV, "r");
    if (pontArqCSV == NULL)
    {
        mensagem_erro();
        return;
    }

    // Modo de escrita (binário)
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

    // Definindo status como inconsistente (0)
    headerCSV = header_set(headerCSV, 0, -2, -2, -2, -2,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    arqBIN_escrever_header(pontArqBin, headerCSV, true);

    // Lendo dados do .csv e escrevendo no .bin
    DADO *dadoCSV = NULL;                            // Ponteiro para dado (reutilizável)
    int quantRegDados = 0;                           // Contador de dados
    long int byteOffsetPonteiro = BYTEOFFSET_HEADER; // Inicializado com o tamanho do header
    fseek(pontArqCSV, 1, SEEK_CUR);                  // Avança para a próxima linha (1º dado)

    while (true)
    {
        // Lê uma linha do csv
        dadoCSV = arqCSV_ler_dado(pontArqCSV);
        if (dadoCSV == NULL)
            break;

        // Escreve a linha no .bin
        arqBIN_escrever_dado(pontArqBin, dadoCSV);
        // Atualizando quantidade de dados e ponteiro
        quantRegDados++;
        byteOffsetPonteiro += dado_get_int(dadoCSV, 3) + 5;

        // Resetando a struct
        dado_apagar(&dadoCSV);
    }
    // Atualizando campos do header
    headerCSV = header_set(headerCSV, 1, -2, byteOffsetPonteiro, quantRegDados, -2,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // Escrevendo o header no arquivo binário
    fseek(pontArqBin, 0, SEEK_SET);
    arqBIN_escrever_header(pontArqBin, headerCSV, false);

    // Apagando structs e fechando os arquivos
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

    // Abrindo arquivo binário no modo de leitura
    FILE *pontArqBin = fopen(nomeArqBin, "rb");
    if (pontArqBin == NULL)
    {
        mensagem_erro();
        return;
    }

    // Posiciona o ponteiro no início do arquivo
    fseek(pontArqBin, 0, SEEK_SET);

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

    // Abrindo arquivo binário no modo de leitura
    FILE *pontArqBin = fopen(nomeArqBin, "rb");
    if (pontArqBin == NULL)
    {
        mensagem_erro();
        return;
    }

    // Lendo quantidade de buscas a serem feitas
    int quantBuscas;
    scanf("%d", &quantBuscas);

    BUSCA *busca = NULL;           // Ponteiro para struct busca (reutilizável)
    HEADER *headerArq = NULL;      // Ponteiro para header do arq. bin.
    long int byteOffsetEncontrado; // Reutilizável
    DADO *dadoEncontrado = NULL;   // Reutilizável

    // Lendo header do arquivo
    headerArq = header_ler(pontArqBin, headerArq);
    long int byteOffsetFimArq = header_get_longint(headerArq, 2);

    // Lendo cada busca do usuário e efetuando-a
    for (int i = 0; i < quantBuscas; i++)
    {
        byteOffsetEncontrado = -1;
        busca = busca_ler(busca); // Lendo parâmetros da busca

        // Buscando no arq. .bin
        while (byteOffsetEncontrado < byteOffsetFimArq)
        {

            byteOffsetEncontrado = arqBIN_buscar_byteOffset(pontArqBin, busca, headerArq, byteOffsetEncontrado);
            if (byteOffsetEncontrado == -1)
            {
                mensagem_regInexistente(); // Não foi encontrado um dado que obedece os campos de busca
                break;
            }
            else if (byteOffsetEncontrado == -2 || byteOffsetEncontrado == -3)
                break; // A busca chegou ao fim do arq.
            else
            {
                // Lê o dado no byteOffset encontrado
                dadoEncontrado = dado_ler(pontArqBin, dadoEncontrado, byteOffsetEncontrado);
                // Atualizando (avançando no arq.) com o tam. do dado
                byteOffsetEncontrado += dado_get_int(dadoEncontrado, 3) + 5;
                // Imprime o dado encontrado
                dado_imprimir(headerArq, dadoEncontrado);
                printf("\n");
                // Reseta a struct
                dado_apagar(&dadoEncontrado);

                // Busca pelo campo idAttack e foi encontrado, para a busca
                if (busca_get_quaisCampos(busca, 0) == 1)
                    break;
            }
        }
        printf("**********\n");

        // Reseta a struct busca
        busca_apagar(&busca);
    }

    // Apagando header e fechando arquivo
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

    // Lendo quant. de buscas a serem feitas
    int quantBuscas;
    scanf(" %d", &quantBuscas);

    // Abrindo o arquivo binário no modo de leitura e escrita
    FILE *pontArqBin = fopen(nomeArqBin, "rb+");
    if (pontArqBin == NULL)
    {
        mensagem_erro();
        return;
    }

    BUSCA *busca = NULL; // Reutilizável
    HEADER *headerArq = NULL;
    headerArq = header_ler(pontArqBin, headerArq);
    if (headerArq == NULL)
    {
        mensagem_erro();
        return;
    }

    long int byteOffsetEncontrado = -1;
    long int byteOffsetFimArq = header_get_longint(headerArq, 2);

    // Lendo e realizando cada busca
    for (int i = 0; i < quantBuscas; i++)
    {
        busca = busca_ler(busca); // Lendo parâmetros da busca

        // Buscar registro que satisfaz os campos de busca
        while (byteOffsetEncontrado < byteOffsetFimArq)
        {
            byteOffsetEncontrado = arqBIN_buscar_byteOffset(pontArqBin, busca, headerArq, byteOffsetEncontrado);
            if (byteOffsetEncontrado < 0)
                break; // Não foi encontrado um dado que obedece os campos de busca
            else
                dado_remover(pontArqBin, headerArq, byteOffsetEncontrado);
        }

        // Resetando struct
        busca_apagar(&busca);
    }

    header_apagar(&headerArq);
    fclose(pontArqBin);

    binarioNaTela(nomeArqBin);

    return;
}

/* funcionalidade5()
Insere um dado no arquivo binário
*/
void funcionalidade5()
{
    char nomeArqBin[TAM_MAX_STR];
    ler_nome_arquivo(nomeArqBin);

    // Abrindo o arquivo binário no modo de leitura e escrita
    FILE *pontArqBin = fopen(nomeArqBin, "rb+");
    if (pontArqBin == NULL)
    {
        mensagem_erro();
        return;
    }

    // Quantidade de dados a serem inseridos
    int quantDados;
    scanf("%d", &quantDados);

    char **entrada = NULL;

    for (int i = 0; i < quantDados; i++)
    {
        // Lê a entrada do usuário para inserir um novo registro
        entrada = ler_entrada_insert();

        if (entrada == NULL)
        {
            mensagem_erro();
            fclose(pontArqBin);
            return;
        }

        if (!arqBIN_insert_dado(pontArqBin, entrada))
        {
            mensagem_erro();
            apaga_entrada(&entrada);
            fclose(pontArqBin);
            return;
        }

        // Libera a memória alocada para a entrada
        apaga_entrada(&entrada);
    }

    fclose(pontArqBin);

    binarioNaTela(nomeArqBin);
}

/* funcionalidade6()
Atualiza um dado no arquivo binário
*/
void funcionalidade6()
{
    char nomeArqBin[TAM_MAX_STR];
    ler_nome_arquivo(nomeArqBin);

    // Abrindo o arquivo binário no modo de leitura e escrita
    FILE *pontArqBin = fopen(nomeArqBin, "rb+");
    if (pontArqBin == NULL)
    {
        mensagem_erro();
        return;
    }

    // Quantidade de atualizações a serem feitas
    int quantAtualiz;
    scanf("%d", &quantAtualiz);

    BUSCA *busca = NULL;                // Reutilizável
    BUSCA *camposAtualizados = NULL;    // Reutilizável
    DADO *dado = NULL;                  // Reutilizável
    DADO *dadoAtualizado = NULL;        // Reutilizável
    HEADER *headerArq = NULL;           // Header do arq. bin.
    long int byteOffsetEncontrado = -1; // ByteOffset para a busca

    // Posiciona o ponteiro no início do arquivo
    fseek(pontArqBin, 0, SEEK_SET);
    // Lê o header
    headerArq = header_ler(pontArqBin, headerArq);
    if (headerArq == NULL) // Erro
    {
        mensagem_erro();
        fclose(pontArqBin);
        return;
    }

    for (int i = 0; i < quantAtualiz; i++)
    {
        byteOffsetEncontrado = -1;

        // Lendo a busca a ser feita
        busca = busca_ler(busca);
        // Lendo os parâmetros a serem atualizados
        camposAtualizados = busca_ler(camposAtualizados);

        byteOffsetEncontrado = arqBIN_buscar_byteOffset(pontArqBin, busca, headerArq, byteOffsetEncontrado);

        // Se encontramos o dado
        if (byteOffsetEncontrado >= BYTEOFFSET_HEADER)
        {
            // Lendo o dado encontrado nas duas structs dado
            dado = dado_ler(pontArqBin, dado, byteOffsetEncontrado);
            dadoAtualizado = dado_ler(pontArqBin, dadoAtualizado, byteOffsetEncontrado);
            if (dado == NULL || dadoAtualizado == NULL)
                continue; // Erro, continua para a próxima iteração

            // Atualizando somente uma struct dado
            dadoAtualizado = busca_atualizar_dado(camposAtualizados, dadoAtualizado);

            // Dado atualizado maior que dado original
            if (dado_get_int(dadoAtualizado, 3) > dado_get_int(dado, 3))
            {
                /*REMOÇÃO POR BYTE OFFFSET*/
            }
        }

        busca_apagar(&busca);
        busca_apagar(&camposAtualizados);
        dado_apagar(&dado);
        dado_apagar(&dadoAtualizado);
    }

    return;
}