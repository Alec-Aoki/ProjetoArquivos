#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "registros.h"

/* header_criar():
Cria uma struct do tipo HEADER e a inicializa
Parâmetros: void
Retorna: ponteiro para a struct do tipo HEADER
*/
HEADER* header_criar(void){
    HEADER* novoHeader = (HEADER *) malloc(sizeof(HEADER)); // Alocando dinâmicamente uma struct do tipo HEADER
    if(novoHeader == NULL){
        printf("Erro ao criar struct\n");

        return NULL;
    }

    // Inicializando a struct
    novoHeader->status = '0';
    novoHeader->topo = -1;
    novoHeader->proxByteOffset = 0;
    novoHeader->nroRegArq = 0;
    novoHeader->nroRegRem = 0;

    return novoHeader; // Retornando ponteiro para HEADER
}

/* header_apagar():
Desaloca uma struct do tipo header e define seu ponteiro para NULL
Parâmetros: ponteiro de ponteiro para a struct a ser desalocada
Retorna: void
*/
void header_apagar(HEADER** header){
    free(*header); // Desalocando memória
    *header = NULL; // Definindo o ponteiro para NULL

    return;
}

/* header_escrever():
Cria um arquivo binário e o inicializa com o header
Parâmetros: ponteiro para uma string (nome do arquivo a ser criado), ponteiro para um header
Retorna:
    Caso bem-sucedido: true
    Caso contrário: false
*/
bool header_escrever(char* nomeArquivo, HEADER* headerArq){
    if(nomeArquivo == NULL){
        printf("Erro com o ponteiro para o nome do arquivo\n");

        return false;
    }

    FILE* pontArq = fopen(nomeArquivo, "wb"); // Cria um arquivo binário para gravação. Caso já exista, sobrescreve
    if(pontArq == NULL){
        printf("Erro ao criar arquivo\n");

        return false;
    }

    // Escrevendo struct header no arquivo campo a campo
    fwrite(&(headerArq->status), sizeof(char), 1, pontArq);
    fwrite(&(headerArq->topo), sizeof(long int), 1, pontArq);
    fwrite(&(headerArq->proxByteOffset), sizeof(long int), 1, pontArq);
    fwrite(&(headerArq->nroRegArq), sizeof(int), 1, pontArq);
    fwrite(&(headerArq->nroRegRem), sizeof(int), 1, pontArq);

    char* stringTemp = (char *) malloc(68 * sizeof(char)); // Alocando dinâmicamente uma string de tamanho máximo de 68 caracteres
    // Essa string será usada para escrever os campos restantes do header (semânticos)
    
    // descreveIdentificador: descrição do campo idAttack
    strcpy(stringTemp, "IDENTIFICADOR DO ATAQUE");
    fwrite(stringTemp, sizeof(char), 23, pontArq);

    // descreveYear: descrição do campo year
    strcpy(stringTemp, "ANO EM QUE O ATAQUE OCORREU");
    fwrite(stringTemp, sizeof(char), 27, pontArq);

    // descreveFinancialLoss: descrição do campo financialLoss
    strcpy(stringTemp, "PREJUIZO CAUSADO PELO ATAQUE");
    fwrite(stringTemp, sizeof(char), 28, pontArq);

    // codDescreveCountry: código da keyword que representa o campo country
    strcpy(stringTemp, "1");
    fwrite(stringTemp, sizeof(char), 1, pontArq);

    // codDescreveType: código da keyword que representa o campo type
    strcpy(stringTemp, "2");
    fwrite(stringTemp, sizeof(char), 1, pontArq);

    // descreveType: descrição do campo type
    strcpy(stringTemp, "TIPO DE AMEACA A SEGURANCA CIBERNETICA");
    fwrite(stringTemp, sizeof(char), 38, pontArq);

    // codDescreveTargetIndustry: código da keyword que representa o campo targetIndustry
    strcpy(stringTemp, "3");
    fwrite(stringTemp, sizeof(char), 1, pontArq);

    // descreveTargetIndustry: descrição do campo targetIndustry
    strcpy(stringTemp, "SETOR DA INDUSTRIA QUE SOFREU O ATAQUE");
    fwrite(stringTemp, sizeof(char), 38, pontArq);

    // codDescreveDefense: código da keyword que representa o campo defenseMechanism
    strcpy(stringTemp, "4");
    fwrite(stringTemp, sizeof(char), 1, pontArq);

    // descreveDefense: descrição do campo defenseMechanism
    strcpy(stringTemp, "ESTRATEGIA DE DEFESA CIBERNETICA EMPREGADA PARA RESOLVER O PROBLEMA");
    fwrite(stringTemp, sizeof(char), 67, pontArq);

    // Alterando o status do arquivo
    strcpy(stringTemp, "1");
    fseek(pontArq, 0, SEEK_SET); // Posicionando pontArq no início do arquivo (campo de status)
    fwrite(stringTemp, sizeof(char), 1, pontArq); // Mudando o status de 0 (incosistente) para 1 (consistente)

    free(stringTemp); // Desalocando a string temporária
    fclose(pontArq); // Fechando o arquivo

    return true;
}

