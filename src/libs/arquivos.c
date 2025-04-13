#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "registros.h"
#include "arquivos.h"

// EM CONSTRUÇÃO
bool arquivo_criar(char* nomeArqBin, char* nomeArqCSV) {
    /* TRATAMENTO DE ERROS E ABERTURA DO ARQUIVO */
    if(nomeArqBin == NULL || nomeArqCSV == NULL) {
        printf("Erro com o ponteiro para o nome do arquivo\n");
        return false;
    }

    FILE* pontArqBin = fopen(nomeArqBin, "wb");
    if(pontArqBin == NULL) {
        printf("Erro ao criar arquivo binário\n");
        return false;
    }

    FILE* pontArqCSV = fopen(nomeArqCSV, "r");
    if(pontArqCSV == NULL) {
        printf("Erro ao abrir arquivo .csv\n");
        fclose(pontArqBin);
        return false;
    }

    HEADER* headerArq = header_criar();
    if(headerArq == NULL) {
        printf("Erro ao criar header\n");
        fclose(pontArqBin);
        fclose(pontArqCSV);
        return false;
    }

    /* ESCREVE HEADER INICIAL */
    header_escrever(pontArqBin, headerArq, true);

    char *campos[7];
    char buffer[256] = "";
    // Posiciona o ponteiro no inicio do arquivo
    fseek(pontArqCSV, 0, SEEK_SET);
    // Pega a primeira linha do arquivo
    fgets(buffer, sizeof(buffer), pontArqCSV);
    printf("%s\n", buffer);
    while (fgets(buffer, sizeof(buffer), pontArqCSV) != NULL){
        // Pega a primeira string antes da virgula
        int fimDaLinha = strcspn(buffer, "\n");
        buffer[fimDaLinha] = '\0';

        char *tok = strtok(buffer, ",");
        int i = 0;
        while (tok != NULL && i < 7) {
            campos[i] = tok;
            tok = strtok(NULL, ",");
            i++;
        }
        DADO *RegTemp = dado_criar(0, 0, -1, atoi(campos[0]), atoi(campos[1]), atof(campos[2]), campos[3], campos[4], campos[5], campos[6]);
        imprime(RegTemp);

    }



    /* ATUALIZAR HEADER */
    header_set_status(headerArq, '1');
    header_escrever(pontArqBin, headerArq, false);

    /* LIMPEZA */
    header_apagar(&headerArq);
    fclose(pontArqBin);
    fclose(pontArqCSV);

    return true;
}
