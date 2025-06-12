#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../registros/header.h"
#include "../../registros/dados.h"
#include "../../auxiliar/auxiliar.h"
#include "../arqCSV.h"

/*arqCSV_ler_header():
Lê o header de um arquivo .csv e cria uma struct do tipo header com os dados lidos
Parâmetro: ponteiro para arquivo
Retorno: ponteiro para struct do tipo header (NULL se mal sucedido)
*/
HEADER *arqCSV_ler_header(FILE *pontArqCSV)
{
    if (pontArqCSV == NULL)
    {
        mensagem_erro();
        return NULL;
    }
    /* LEITURA DOS CAMPOS DO HEADER */
    // Vetor de ponteiros de strings para guardar os campos do header e dos dados
    char *campos[7];
    // Buffer para leitura do header e dos campos do .csv
    char buffer[256] = "";

    // Posiciona o ponteiro no inicio do arquivo
    fseek(pontArqCSV, 0, SEEK_SET);

    // Lê a primeira linha do .csv (descrições semânticas do header)
    fread(buffer, 253, 1, pontArqCSV);
    // Substituindo '\n' por '\0' no buffer, por segurança
    buffer[253] = '\0';

    // Vamos utilizar o strtok para separar a string no buffer nas ','
    // e evitar alocar espaço para as strings do header
    char *tok = strtok(buffer, ",");
    // Guarda cada parte da string em um dos campos do vetor campos
    int i = 0;
    while (tok != NULL && i < 7)
    {
        campos[i] = tok;
        tok = strtok(NULL, ",");
        i++;
    }

    // Criando struct header com os campos semânticos do header do .csv
    HEADER *headerArq = NULL;
    headerArq = header_set(headerArq, -2, -2, -2, -2, -2, campos[0],
                           campos[1], campos[2], campos[3],
                           campos[4], campos[5], campos[6]);
    if (headerArq == NULL)
    {
        mensagem_erro();
        return NULL;
    }

    return headerArq;
}

/*arqCSV_ler_dado():
Lê um dado de um arquivo .csv
Parâmetro: ponteiro para arquivo
Retorno: ponteiro para struct do tipo dado (NULL se mal sucedido)
*/
DADO *arqCSV_ler_dado(FILE *pontArqCSV)
{
    if (pontArqCSV == NULL)
    {
        mensagem_erro();
        return NULL;
    }

    /* LEITURA DOS DADOS */
    // Vetor de ponteiros de strings para guardar os campos do header e dos dados
    char *campos[7];
    // Buffer para leitura do header e dos campos do .csv
    char buffer[256] = "";

    if (fgets(buffer, sizeof(buffer), pontArqCSV) == NULL)
        return NULL;

    // Identifica o fim da linha lida e substitui o "\n" por um '\0'
    int fimDaLinha = strcspn(buffer, "\n");
    buffer[fimDaLinha] = '\0';

    // Inicializando vetor de campos
    for (int j = 0; j < 7; j++)
    {
        campos[j] = "";
    }

    // Variáveis necessárias para a leitura do arquivo
    // Ponteiro de char que aponta para o início da string armazenada do buffer
    char *ptr = buffer;
    // Ponteiro de char que receberá as strings entre as vírgulas do csv
    char *campo;
    // Contador do loop para acessar os campos do array
    int i = 0;

    // Loop que separa os campos da linha lida
    while ((campo = strsep(&ptr, ",")) != NULL)
    {
        // Guarda a string num vetor de strings se essa não é nula
        if (campo != 0)
        {
            campos[i] = campo;
            i++;
        }
    }

    // Guarda os dados lidos na struct DADO
    DADO *dadoArq = dado_set(NULL, 0, 0, -1, str_to_int(campos[0]), str_to_int(campos[1]),
                             str_to_float(campos[2]), campos[3], campos[4],
                             campos[5], campos[6]);
    if (dadoArq == NULL)
    {
        mensagem_erro();
        return NULL;
    }

    return dadoArq;
}