#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "auxiliar.h"
#include "registros.h"
#include "arquivos.h"

/* arquivo_criar()
Transforma um arquivo .csv em .bin
Parâmetros: ponteiros para strings (nomes dos arquivos)
Retorna:
*/
void arquivo_criar(char* nomeArqBin, char* nomeArqCSV){
    // Abre o arquivo .csv para leitura
    FILE* pontArqCSV = fopen(nomeArqCSV, "r");
    if(pontArqCSV == NULL) return;

    if(nomeArqBin == NULL) return;

    // Cria um arquivo binário para gravação. Caso já exista, sobrescreve
    FILE* pontArqBin = fopen(nomeArqBin, "wb");
    if(pontArqBin == NULL) return;
    
    /* LEITURA DOS CAMPOS DO HEADER */
    char *campos[7]; // Vetor de ponteiros de strings para guardar os campos do header e dos dados
    char buffer[256] = ""; // Buffer para leitura do header e dos campos do .csv

    fseek(pontArqCSV, 0, SEEK_SET); // Posiciona o ponteiro no inicio do arquivo
    
    fread(buffer, 253, 1, pontArqCSV); // Lê a primeira linha do .csv (descrições semânticas do header)
    buffer[253] = '\0'; // Substituindo '\n' por '\0' no buffer, por segurança

    // Vamos utilizar o strtok para separar a string no buffer nas ',' e evitar alocar espaço para as strings do header
    char *tok = strtok(buffer, ",");
    // Guarda cada parte da string em um dos campos do vetor campos
    int i = 0;
    while(tok != NULL && i < 7){
        campos[i] = tok;
        tok = strtok(NULL, ",");
        i++;
    }

    // Criando struct header com os campos semânticos do header do .csv
    HEADER *headerArq = header_criar(campos[0], campos[1], campos[2], campos[3], campos[4], campos[5], campos[6]);
    if(headerArq == NULL) return;

    // Escrevendo header no arquivo binário
    header_escrever(pontArqBin, headerArq, true); 

    /* LEITURA DOS DADOS */
    // Posiciona o ponteiro no inicio do arquivo
    fseek(pontArqCSV, 0, SEEK_SET);

    // Pega a primeira linha do arquivo e ignora
    fgets(buffer, sizeof(buffer), pontArqCSV);

    // Contadores para atualizar os dados do header
    int quantRegDados = 0;
    int byteOffset = 276; // Inicializado com o tamanho do header

    while (fgets(buffer, sizeof(buffer), pontArqCSV) != NULL){
        // Identifica o fim da linha lida e substitui o "\n" por um '\0'
        int fimDaLinha = strcspn(buffer, "\n");
        buffer[fimDaLinha] = '\0';

        for (int j = 0; j < 7; j++) {
            campos[j] = "";
        }

        // Variáveis necessárias para a leitura do arquivo
        char *ptr = buffer;     // Ponteiro de char que aponta para o início da string armazenada do buffer
        char *campo;            // Ponteiro de char que receberá as strings entre as vírgulas do csv
        int i = 0;              // Contador do loop para acessar os campos do array

        // Loop que separa os campos da linha lida
        while ((campo = strsep(&ptr, ",")) != NULL) {
            // Guarda a string num vetor de strings se essa não é nula
            if (campo != 0) {
                campos[i] = campo;
                i++;
            }
        }

        // Guarda os dados lidos na struct DADO
        DADO *RegTemp = dado_criar('0', 0, -1, str_to_int(campos[0]), str_to_int(campos[1]), str_to_float(campos[2]), campos[3], campos[4], campos[5], campos[6]);

        quantRegDados++; // Incrementando a quantidade de registros no arquivo
        byteOffset += dado_get_tamanho(RegTemp);

        // Escreve os registros no arquivo binario
        dado_escrever(pontArqBin, RegTemp);
        dado_apagar(&RegTemp);
    }

    /* ATUALIZANDO CAMPOS DO HEADER */
    // Alterando o status do arquivo antes de fechá-lo
    header_set_status(headerArq, '1');
    header_set_nroRegArq(headerArq, quantRegDados);
    header_set_proxByteOffset(headerArq, byteOffset);

    /* ESCREVENDO HEADER ATUALIZADO NO ARQUIVO */
    header_escrever(pontArqBin, headerArq, false); // Não precisamos escrever os campos semânticos novamente

    /* DESALOCANDO MEMÓRIA E FECHANDO O ARQUIVO */
    header_apagar(&headerArq); // Desalocando struct header
    fclose(pontArqBin); // Fechando o arquivo

    return;
}

/*TODO*/
void arquivo_imprimir(char* nomeArqBin){
    if(nomeArqBin == NULL) return; // Erro com nome do arquivo binário a ser aberto

    FILE* pontArqBin = fopen(nomeArqBin, "rb"); // Abrindo arquivo binário no modo de leitura
    if(pontArqBin == NULL) return; // Erro com a abertura do arquivo

    fseek(pontArqBin, 0, SEEK_SET); // Posiciona o ponteiro no início do arquivo

    HEADER* header = header_ler(pontArqBin, NULL);
    DADO* dado = dado_ler(pontArqBin, NULL);
    dado_imprimir(header, dado);
    printf("\n");

    // Atualizar isso aqui mais tarde
    while(pontArqBin != EOF){
        dado_ler(pontArqBin, dado);
        dado_imprimir(header, dado);
        printf("\n");
    }

    dado_apagar(&dado);
    header_apagar(&header);
    return;
}