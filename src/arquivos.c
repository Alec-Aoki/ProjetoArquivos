#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "arquivos.h"

/*Armazena os campos do header que sofrem alterações*/
struct header_{
    char status; // Consistência do arquivo de dados. 0 = inconsistente, 1 = consistente
    long int topo; // Byteoffset do primeiro registro logicamente removido. -1 = nenhum registro removido
    long int proxByteOffset; // Valor do próximo byteoffset disponível
    int nroRegArq; // Quantidade de registros não removidos
    int nroRegRem; // Quantidade de registros removidos
};

/*Armazena os campos de um registro de dados*/
struct dados_ {
    char removido; // Indica se o registro está logicamente removido. 1 = removido, 0 = não removido
    int tamanhoRegistro; // Tamanho do registro em bytes
    long int prox; // Byteoffset do próximo registro logicamente removido. Inicializado com -1
    int idAttack; // Código identificador do ataque
    int year; // Ano em que o ataque ocorreu
    float financialLoss; // Prejuízo causado pelo ataque
};

/*FUNÇÕES AUXILIARES*/
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

/* arquivo_criar():
Cria um arquivo binário e o inicializa com o header
Parâmetros: ponteiro para uma string (nome do arquivo a ser criado)
Retorna:
    Caso bem-sucedido: true
    Caso contrário: false
*/
bool arquivo_criar(char* nomeArquivo){
    if(nomeArquivo == NULL){
        printf("Erro com o ponteiro para o nome do arquivo\n");

        return false;
    }

    FILE* pontArq = fopen(nomeArquivo, "wb"); // Cria um arquivo binário para gravação. Caso já exista, sobrescreve
    if(pontArq == NULL){
        printf("Erro ao criar arquivo\n");

        return false;
    }

    HEADER* headerTemp = header_criar();
    if(headerTemp == NULL){
        printf("Erro ao criar struct\n");

        return false;
    }

    // Escrevendo struct header no arquivo campo a campo
    fwrite(&(headerTemp->status), sizeof(char), 1, pontArq);
    fwrite(&(headerTemp->topo), sizeof(long int), 1, pontArq);
    fwrite(&(headerTemp->proxByteOffset), sizeof(long int), 1, pontArq);
    fwrite(&(headerTemp->nroRegArq), sizeof(int), 1, pontArq);
    fwrite(&(headerTemp->nroRegRem), sizeof(int), 1, pontArq);

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
    free(headerTemp); // Desalocando struct HEADER criada
    fclose(pontArq); // Fechando o arquivo

    return true;
}