#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "registros.h"
#include "arquivos.h"

// Protótipos das funções
int str_to_int (char *str);
float str_to_float (char *str);





/* arquivo_criar():
Função que cria um arquivo binário e guarda nele os dados de um aquivo csv
Parâmetros: Strigs com o nome dos arquivos
Retorno: booleano indicando status da operação
*/
bool arquivo_criar(char* nomeArqBin, char* nomeArqCSV) {
    /* TRATAMENTO DE ERROS E ABERTURA DO ARQUIVO */
    if(nomeArqBin == NULL || nomeArqCSV == NULL) {
        printf("Erro com o ponteiro para o nome do arquivo\n");
        return false;
    }

    // Abre arquivo binário, se existir, senão cria um para escrita
    FILE* pontArqBin = fopen(nomeArqBin, "wb");
    if(pontArqBin == NULL) {
        printf("Erro ao criar arquivo binário\n");
        return false;
    }

    // Abre um arquivo para leitura
    FILE* pontArqCSV = fopen(nomeArqCSV, "r");
    if(pontArqCSV == NULL) {
        printf("Erro ao abrir arquivo .csv\n");
        fclose(pontArqBin);
        return false;
    }

    // Cria um estrutura HEADER
    HEADER* headerArq = header_criar();
    if(headerArq == NULL) {
        printf("Erro ao criar header\n");
        fclose(pontArqBin);
        fclose(pontArqCSV);
        return false;
    }

    /* ESCREVE HEADER INICIAL */
    header_escrever(pontArqBin, headerArq, true);

    char *campos[7];        // Array de ponteiros para strings que guardará os dados lidos do csv
    char buffer[256] = "";  // Inicializa o buffer com strig nula

    // Posiciona o ponteiro no inicio do arquivo csv
    fseek(pontArqCSV, 0, SEEK_SET);

    // Pega a primeira linha do arquivo
    fgets(buffer, sizeof(buffer), pontArqCSV);

    // Loop que lê linha a linha do arquivo csv
    while (fgets(buffer, sizeof(buffer), pontArqCSV) != NULL){

        // Identifica o fim da linha lida e substitui o "\n" por um '\0'
        int fimDaLinha = strcspn(buffer, "\n");
        buffer[fimDaLinha] = '\0';

        // Inicializa os campos com strings nulas
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
        DADO *RegTemp = dado_criar(0, 0, -1, str_to_int(campos[0]), str_to_int(campos[1]), str_to_float(campos[2]), campos[3], campos[4], campos[5], campos[6]);
        // Calcula o tamanho do registro
        set_dado_reg_tam(RegTemp);

        // Apenas para teste
        /*---------------*/
        imprime(RegTemp);
        /*---------------*/

        // Guarda os dados da struct num arquivo binário
        guarda_arqBin(pontArqBin, RegTemp);
    }



    /* ATUALIZAR HEADER */
    header_set_status(headerArq, '1');
    header_escrever(pontArqBin, headerArq, false);

    /* LIMPEZA */
    header_apagar(&headerArq);

    /* FECHAMENTO DOS ARQUIVOS */
    fclose(pontArqBin);
    fclose(pontArqCSV);

    return true;
}


//  Funções auxiliares:
    
/* str_to_int():
A função converte uma string numérica para um intero, caso nula -1
Parâmetros: uma string
Retorno : o inteiro correspondente, ou -1 
*/
int str_to_int (char *str) {
    if (str == NULL)
        return -1;
    
    if (strcmp(str, "") == 0)
        return -1;

    return atoi(str);
}
    
/* str_to_float():
A função converte uma string numérica para um float, caso nula -1
Parâmetros: uma string
Retorno : o float correspondente, ou -1 
*/
float str_to_float (char *str) {
    if (str == NULL)
        return -1;
    
    if (strcmp(str, "") == 0)
        return -1;

    return atof(str);    
}
