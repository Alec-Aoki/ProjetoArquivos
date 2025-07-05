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
#include "../arvB.h"
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

    // Definindo status como inconsistente (0)
    headerCSV = header_set(headerCSV, 0, -2, -2, -2, -2,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    // Escrevendo o header  no arquivo binário
    fseek(pontArqBin, 0, SEEK_SET);
    header_escrever(pontArqBin, headerCSV, true);

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
        dado_escrever(pontArqBin, dadoCSV, 0);
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
    header_escrever(pontArqBin, headerCSV, false);

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
            byteOffsetEncontrado = arqBIN_buscar_byteOffset(pontArqBin, busca,
                                                            headerArq, byteOffsetEncontrado);
            if (byteOffsetEncontrado == -1)
            {
                // Não foi encontrado um dado que obedece os campos de busca
                mensagem_regInexistente();
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
    // Definindo status como inconsistente (0)
    headerArq = header_set(headerArq, 0, -2, -2, -2, -2,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    // Escrevendo o header  no arquivo binário
    fseek(pontArqBin, 0, SEEK_SET);
    header_escrever(pontArqBin, headerArq, false);

    long int byteOffsetEncontrado = -1;
    long int byteOffsetFimArq = header_get_longint(headerArq, 2);

    // Lendo e realizando cada busca
    for (int i = 0; i < quantBuscas; i++)
    {
        busca = busca_ler(busca); // Lendo parâmetros da busca

        // Buscar registro que satisfaz os campos de busca
        while (byteOffsetEncontrado < byteOffsetFimArq)
        {
            byteOffsetEncontrado = arqBIN_buscar_byteOffset(pontArqBin, busca,
                                                            headerArq, byteOffsetEncontrado);
            if (byteOffsetEncontrado < 0)
                break; // Não foi encontrado um dado que obedece os campos de busca
            else
                dado_remover(pontArqBin, headerArq, byteOffsetEncontrado);
        }

        // Resetando struct
        busca_apagar(&busca);
    }

    // Definindo status como consistente (1)
    headerArq = header_set(headerArq, 1, -2, -2, -2, -2,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    // Escrevendo o header  no arquivo binário
    fseek(pontArqBin, 0, SEEK_SET);
    header_escrever(pontArqBin, headerArq, false);

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
    scanf(" %d", &quantDados);

    HEADER *headerArq = NULL;
    headerArq = header_ler(pontArqBin, headerArq);
    if (headerArq == NULL)
    {
        mensagem_erro();
        fclose(pontArqBin);
        return;
    }
    // Definindo status como inconsistente (0)
    headerArq = header_set(headerArq, 0, -2, -2, -2, -2,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    // Escrevendo o header  no arquivo binário
    fseek(pontArqBin, 0, SEEK_SET);
    header_escrever(pontArqBin, headerArq, false);

    for (int i = 0; i < quantDados; i++)
    {
        char **entrada = NULL;
        DADO *dado = NULL;

        // Lê a entrada do usuário para inserir um novo registro
        entrada = ler_entrada_insert();
        if (entrada == NULL)
        {
            mensagem_erro();
            fclose(pontArqBin);
            return;
        }

        dado = dado_set(dado, 0, 0, -1, str_to_int(entrada[0]),
                        str_to_int(entrada[1]), str_to_float(entrada[2]),
                        entrada[3], entrada[4], entrada[5], entrada[6]);

        if (!arqBIN_insert_dado(pontArqBin, headerArq, dado))
        {
            mensagem_erro();
            apaga_entrada(&entrada);
            fclose(pontArqBin);
            return;
        }

        // Libera a memória alocada para a entrada
        apaga_entrada(&entrada);
        dado_apagar(&dado);
    }

    // Atualizando o header do arquivo binário
    // Definindo status como consistente (1)
    headerArq = header_set(headerArq, 1, -2, -2, -2, -2,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    // Escrevendo o header  no arquivo binário
    fseek(pontArqBin, 0, SEEK_SET);
    header_escrever(pontArqBin, headerArq, false);

    // Apagando structs e fechando o arquivo
    header_apagar(&headerArq);
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

    // Definindo status como inconsistente (0)
    headerArq = header_set(headerArq, 0, -2, -2, -2, -2,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    // Escrevendo o header  no arquivo binário
    fseek(pontArqBin, 0, SEEK_SET);
    header_escrever(pontArqBin, headerArq, false);

    for (int i = 0; i < quantAtualiz; i++)
    {
        byteOffsetEncontrado = -1;

        // Lendo a busca a ser feita
        busca = busca_ler(busca);
        // Lendo os parâmetros a serem atualizados
        camposAtualizados = busca_ler(camposAtualizados);

        byteOffsetEncontrado = arqBIN_buscar_byteOffset(pontArqBin, busca,
                                                        headerArq, byteOffsetEncontrado);

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
                dado_remover(pontArqBin, headerArq, byteOffsetEncontrado);

                arqBIN_insert_dado(pontArqBin, headerArq, dadoAtualizado);
            }
            // Dado atualizado com tamanho menor ou igual ao original
            else
            {
                // Calculando quantidade de lixo
                int quantLixo = dado_get_int(dado, 3) - dado_get_int(dadoAtualizado, 3);
                // Definindo tamanho do dado atualizado para o tam. do dado orig.
                dadoAtualizado = dado_set(dadoAtualizado, -2, dado_get_int(dado, 3),
                                          -2, -2, -2, -2, NULL, NULL, NULL, NULL);
                // Posicionando ponteiro no início do dado
                fseek(pontArqBin, byteOffsetEncontrado, SEEK_SET);
                // Sobrescrevendo dado
                dado_escrever(pontArqBin, dadoAtualizado, quantLixo);
            }
        }

        busca_apagar(&busca);
        busca_apagar(&camposAtualizados);
        dado_apagar(&dado);
        dado_apagar(&dadoAtualizado);
    }
    // Definindo status como consistente (1)
    headerArq = header_set(headerArq, 1, -2, -2, -2, -2,
                           NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    // Escrevendo o header  no arquivo binário
    fseek(pontArqBin, 0, SEEK_SET);
    header_escrever(pontArqBin, headerArq, false);

    header_apagar(&headerArq);
    fclose(pontArqBin);

    binarioNaTela(nomeArqBin);

    return;
}

/* funcionalidade7():
Esta funcionalidade cria um arquivo de índice árvore-B e insere as chaves
*/
void funcionalidade7()
{
    // 1. Lê o nome arquivo de dados e arquivo de indices
    char nomeArqDados[TAM_MAX_STR];
    char nomeArqArvB[TAM_MAX_STR];

    ler_nome_arquivo(nomeArqDados);
    ler_nome_arquivo(nomeArqArvB);

    /*Abrindo arquivos*/
    // Modo de leitura (binário)
    FILE *pontArqDados = fopen(nomeArqDados, "rb");
    if (pontArqDados == NULL)
    {
        mensagem_erro();
        return;
    }

    // Modo de escrita (binário)
    FILE *pontArqArv = fopen(nomeArqArvB, "w+b");
    if (pontArqArv == NULL)
    {
        mensagem_erro();
        return;
    }

    // 2. inserir um-a-um os índices sendo eles indexados pelo idAttack (Registros logicamente removidos não devem ser inseridos)
    HEADER_ARVB *headerArv = ArvB_header_criar();          // Header da árvore
    headerArv = ArvB_header_set(headerArv, 0, -2, -2, -2); // Definindo status do arq. da arv. como inconsistente
    ArvB_header_escrever(pontArqArv, headerArv);           // Escrevendo header

    HEADER *headerDados = header_criar();
    headerDados = header_ler(pontArqDados, headerDados); // Header do arq. de dados

    int quantReg = header_get_int(headerDados, 1);
    int byteOffsetDado = BYTEOFFSET_HEADER; // Contador para percorrer o arquivo de dados

    DADO *dadoTemp = NULL;

    // Loop de leitura de dados
    while (quantReg > 0)
    {
        // Lê dado do arq. bin
        dadoTemp = dado_ler(pontArqDados, dadoTemp, byteOffsetDado);
        if (dadoTemp == NULL)
        {
            mensagem_erro();
            return; // Erro ao ler o dado
        }

        // Inserindo o dado somente se ele não estiver removido
        if (dado_get_removido(dadoTemp) != '1')
        {
            ArvB_inserir(pontArqArv, headerArv, dado_get_int(dadoTemp, 1), byteOffsetDado); // Atualiza o header automaticamente
            quantReg--;
        }

        // Avançando o contador
        byteOffsetDado += dado_get_int(dadoTemp, 3) + 5;
    }

    headerArv = ArvB_header_set(headerArv, 1, -2, -2, -2); // Definindo status do arq. da arv. como consistente
    ArvB_header_escrever(pontArqArv, headerArv);           // Escrevendo header

    // print_arvore(pontArqArv, pontArqDados, TAM_HEADER_ARVB + ArvB_header_get_int(headerArv, 1) * TAM_REGISTRO_ARVB, headerDados);

    dado_apagar(&dadoTemp);
    header_apagar(&headerDados);
    ArvB_header_apagar(&headerArv);
    fclose(pontArqDados);
    fclose(pontArqArv);

    binarioNaTela(nomeArqArvB);

    return;
}

/* funcionalidade8():
Faz a busca de registros que atendam os campos da busca
*/
void funcionalidade8()
{
    char nomeArqDados[TAM_MAX_STR];
    char nomeArqArvB[TAM_MAX_STR];

    ler_nome_arquivo(nomeArqDados);
    ler_nome_arquivo(nomeArqArvB);

    // Abre o arquivo de dados para leitura
    FILE *pontArqDados = fopen(nomeArqDados, "rb");
    if (pontArqDados == NULL)
    {
        mensagem_erro();
        return;
    }

    // Abre o arquivo de índices árvore-B para leitura
    FILE *pontArqArvB = fopen(nomeArqArvB, "rb");
    if (pontArqArvB == NULL)
    {
        mensagem_erro();
        fclose(pontArqDados);
        return;
    }

    // Lendo quantidade de buscas a serem feitas
    int quantBuscas;
    scanf("%d", &quantBuscas);

    BUSCA *busca = NULL;
    HEADER *headerDados = NULL;
    HEADER_ARVB *headerArvB = NULL;
    // Lendo header do arquivo de dados
    headerDados = header_ler(pontArqDados, headerDados);
    if (headerDados == NULL)
    {
        mensagem_erro();
        fclose(pontArqDados);
        return;
    }

    // Lendo header do arquivo de índices árvore-B
    headerArvB = ArvB_header_ler(pontArqArvB, headerArvB);
    if (headerArvB == NULL)
    {
        mensagem_erro();
        fclose(pontArqArvB);
        return;
    }

    // Guarda o byteOffset da raiz
    int byteOffsetRaiz = ArvB_header_get_int(headerArvB, 1);

    // Lendo cada busca do usuário e efetuando-a
    for (int i = 0; i < quantBuscas; i++)
    {
        busca = busca_ler(busca);
        // Verifica se o idAttack faz parte do filtro de busca
        int buscaIdAttack = busca_get_quaisCampos(busca, 1);
        if (buscaIdAttack == 0)
        {
            // Caso faça, faz busca via na árvore de busca
            int idAttack = busca_get_int(busca, 0);
            NO *noEncontrado = NULL;
            noEncontrado = ArvB_busca(pontArqArvB, byteOffsetRaiz, idAttack);
            if (noEncontrado == NULL)
            {
                mensagem_regInexistente();
                continue; // Registro não encontrado
            }

            ArvB_compara_dado(pontArqArvB, noEncontrado, busca);
        }
        else
        {
            // Caso não faça, visita todos os nós da árvore verificandos
            ArvB_DFS(pontArqArvB, pontArqDados, byteOffsetRaiz, busca, headerDados);
        }

        printf("**********\n");

        // Reseta a struct busca
        busca_apagar(&busca);
    }

    header_apagar(&headerDados);
    ArvB_header_apagar(&headerArvB);

    fclose(pontArqDados);
    fclose(pontArqArvB);
}

/* funcionalidade9()
NAO DEVE SER IMPLEMENTADA
*/

/* funcionalidade10():
Insere um registro num arquivo de dados dado
*/
void funcionalidade10()
{
    char nomeArqDados[TAM_MAX_STR];
    char nomeArqArvB[TAM_MAX_STR];

    ler_nome_arquivo(nomeArqDados);
    ler_nome_arquivo(nomeArqArvB);

    // Abre o arquivo de dados para leitura e escrita
    FILE *pontArqDados = fopen(nomeArqDados, "rb+");
    if (pontArqDados == NULL)
    {
        mensagem_erro();
        return;
    }

    // Abre o arquivo de índices árvore-B para leitura e escrita
    FILE *pontArqArvB = fopen(nomeArqArvB, "rb+");
    if (pontArqArvB == NULL)
    {
        mensagem_erro();
        fclose(pontArqDados);
        return;
    }

    // Quantidade de dados a serem inseridos
    int quantDados;
    scanf(" %d", &quantDados);

    HEADER *headerDados = NULL;
    headerDados = header_ler(pontArqDados, headerDados);
    if (headerDados == NULL)
    {
        mensagem_erro();
        fclose(pontArqDados);
        fclose(pontArqArvB);
        return;
    }

    HEADER_ARVB *headerArvB = NULL;
    headerArvB = ArvB_header_ler(pontArqArvB, headerArvB);
    if (headerArvB == NULL)
    {
        mensagem_erro();
        fclose(pontArqDados);
        fclose(pontArqArvB);
        return;
    }

    // Definindo status como inconsistente (0)
    headerDados = header_set(headerDados, 0, -2, -2, -2, -2,
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    // Escrevendo o header  no arquivo binário
    fseek(pontArqDados, 0, SEEK_SET);
    header_escrever(pontArqDados, headerDados, false);

    for (int i = 0; i < quantDados; i++)
    {
        char **entrada = NULL;
        DADO *dado = NULL;

        // Lê a entrada do usuário para inserir um novo registro
        entrada = ler_entrada_insert();
        if (entrada == NULL)
        {
            mensagem_erro();
            fclose(pontArqDados);
            fclose(pontArqArvB);
            return;
        }

        dado = dado_set(dado, 0, 0, -1, str_to_int(entrada[0]),
                        str_to_int(entrada[1]), str_to_float(entrada[2]),
                        entrada[3], entrada[4], entrada[5], entrada[6]);

        if (!arqBIN_insert_dado(pontArqDados, headerDados, dado))
        {
            mensagem_erro();
            apaga_entrada(&entrada);
            fclose(pontArqDados);
            fclose(pontArqArvB);
            return;
        }

        BUSCA *busca = NULL;
        busca = busca_set(busca, str_to_int(entrada[0]), -2, -2,
                          NULL, NULL, NULL, NULL);
        long byteOffset = arqBIN_buscar_byteOffset(pontArqDados, busca,
                                                   headerDados, -1);

        if (byteOffset > 0)
            ArvB_inserir(pontArqArvB, headerArvB, dado_get_int(dado, 1), byteOffset);

        dado_apagar(&dado);
        apaga_entrada(&entrada);
    }

    // Atualizando o header do arquivo binário
    // Definindo status como consistente (1)
    headerDados = header_set(headerDados, 1, -2, -2, -2, -2,
                             NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    // Escrevendo o header  no arquivo binário
    fseek(pontArqDados, 0, SEEK_SET);
    header_escrever(pontArqDados, headerDados, false);

    header_apagar(&headerDados);
    ArvB_header_apagar(&headerArvB);
    fclose(pontArqDados);
    fclose(pontArqArvB);
}

/* funcionalidade11():
Atualiza um registro no arquivo de dados, exceto o campo idAttack
*/