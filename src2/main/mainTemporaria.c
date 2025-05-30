#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../libs/registros/header.h"
#include "../libs/registros/dados.h"
#include "../libs/auxiliar/auxiliar.h"

int main(void)
{
    char nomeArqCSV[256];
    char nomeArqBin[256];

    scanf("%s", nomeArqCSV);
    scanf("%s", nomeArqBin);

    {
        // Abre o arquivo .csv para leitura
        FILE *pontArqCSV = fopen(nomeArqCSV, "r");
        if (pontArqCSV == NULL)
        {
            printf("Falha no processamento do arquivo.\n");
            return 0;
        }

        if (nomeArqBin == NULL)
        {
            printf("Falha no processamento do arquivo.\n");
            return 0;
        }

        // Cria um arquivo binário para gravação. Caso já exista, sobrescreve
        FILE *pontArqBin = fopen(nomeArqBin, "wb");
        if (pontArqBin == NULL)
        {
            printf("Falha no processamento do arquivo.\n");
            return 0;
        }

        /* LEITURA DOS CAMPOS DO HEADER */
        char *campos[7];       // Vetor de ponteiros de strings para guardar os campos do header e dos dados
        char buffer[256] = ""; // Buffer para leitura do header e dos campos do .csv

        fseek(pontArqCSV, 0, SEEK_SET); // Posiciona o ponteiro no inicio do arquivo

        fread(buffer, 253, 1, pontArqCSV); // Lê a primeira linha do .csv (descrições semânticas do header)
        buffer[253] = '\0';                // Substituindo '\n' por '\0' no buffer, por segurança

        // Vamos utilizar o strtok para separar a string no buffer nas ',' e evitar alocar espaço para as strings do header
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
        HEADER *headerArq = header_criar(campos[0], campos[1], campos[2], campos[3], campos[4], campos[5], campos[6]);
        if (headerArq == NULL)
        {
            printf("Falha no processamento do arquivo.\n");
            return 0;
        }

        // Escrevendo header no arquivo binário
        header_escrever(pontArqBin, headerArq, true);

        /* LEITURA DOS DADOS */
        // Posiciona o ponteiro no inicio do arquivo
        fseek(pontArqCSV, 0, SEEK_SET);

        // Pega a primeira linha do arquivo e ignora
        fgets(buffer, sizeof(buffer), pontArqCSV);

        // Contadores para atualizar os dados do header
        int quantRegDados = 0;
        long int byteOffset = BYTEOFFSET_HEADER; // Inicializado com o tamanho do header

        while (fgets(buffer, sizeof(buffer), pontArqCSV) != NULL)
        {
            // Identifica o fim da linha lida e substitui o "\n" por um '\0'
            int fimDaLinha = strcspn(buffer, "\n");
            buffer[fimDaLinha] = '\0';

            for (int j = 0; j < 7; j++)
            {
                campos[j] = "";
            }

            // Variáveis necessárias para a leitura do arquivo
            char *ptr = buffer; // Ponteiro de char que aponta para o início da string armazenada do buffer
            char *campo;        // Ponteiro de char que receberá as strings entre as vírgulas do csv
            int i = 0;          // Contador do loop para acessar os campos do array

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
            DADO *RegTemp = dado_criar(0, 0, -1, str_to_int(campos[0]), str_to_int(campos[1]), str_to_float(campos[2]), campos[3], campos[4], campos[5], campos[6]);

            quantRegDados++; // Incrementando a quantidade de registros no arquivo

            // Escreve os registros no arquivo binario
            dado_escrever(pontArqBin, RegTemp);
            byteOffset += dado_get_tamReg(RegTemp) + 5; // Atualizando byteOffset
            dado_apagar(&RegTemp);
        }

        /* ATUALIZANDO CAMPOS DO HEADER */
        fseek(pontArqCSV, 0, SEEK_SET);
        // Alterando o status do arquivo antes de fechá-lo
        header_set_status(headerArq, '1');
        header_set_nroRegArq(headerArq, quantRegDados);
        header_set_proxByteOffset(headerArq, byteOffset);

        /* ESCREVENDO HEADER ATUALIZADO NO ARQUIVO */
        header_escrever(pontArqBin, headerArq, false); // Não precisamos escrever os campos semânticos novamente

        /* DESALOCANDO MEMÓRIA E FECHANDO O ARQUIVO */
        header_apagar(&headerArq); // Desalocando struct header
        fclose(pontArqBin);        // Fechando o arquivo

        binarioNaTela(nomeArqBin);
    }

    {
        if (nomeArqBin == NULL)
        { // Erro com nome do arquivo binário a ser aberto
            printf("Falha no processamento do arquivo.\n");
            return 0;
        }

        FILE *pontArqBin = fopen(nomeArqBin, "rb"); // Abrindo arquivo binário no modo de leitura
        if (pontArqBin == NULL)
        { // Erro com a abertura do arquivo
            printf("Falha no processamento do arquivo.\n");
            return 0;
        }

        fseek(pontArqBin, 0, SEEK_SET); // Posiciona o ponteiro no início do arquivo

        HEADER *header = header_ler(pontArqBin, NULL); // Leitura do header do arquivo

        int byteOffset = BYTEOFFSET_HEADER; // Inicializado com tamanho do header
        DADO *dado = NULL;

        int quantRegArq = header_get_nroRegArq(header); // Contador para loop de leitura de dados
        if (quantRegArq == 0)
        {
            printf("Registro inexistente.\n");
            return 0;
        }

        // Loop de leitura de dados
        while (quantRegArq > 0)
        {
            dado = dado_ler(pontArqBin, dado, byteOffset);
            // Imprimindo o dado somente se ele não estiver removido
            if (dado_get_removido(dado) != '1')
            {
                dado_imprimir(header, dado);
                printf("\n");
            }

            byteOffset += dado_get_tamReg(dado) + 5;
            quantRegArq--;
        }

        dado_apagar(&dado);
        header_apagar(&header);
        fclose(pontArqBin);
    }

    return 0;
}