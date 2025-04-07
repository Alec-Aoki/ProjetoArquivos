#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "registros.h"

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
    char* country; // País onde ocorreu o ataque
    char* attackType; // Tipo de ameaça à segurança cibernética
    char* targetIndustry; // Setor da indústria que sofreu o ataque
    char* defenseMechanism; // Estratégia de defesa usada para resolver o problema
};

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

/* header_set_status():
Define o campo de status de um header pré-existente
Parâmetros: ponteiro para o header, status a ser definido
Retorna: true se bem sucedido, false senão
*/
bool header_set_status(HEADER* header, char status){
    if(header == NULL){
        printf("Erro ao acessar header\n");
        return false;
    }

    header->status = status; // Definindo o novo status
    return true;
}

/* header_escrever():
Escreve um header passado no arquivo binário
Parâmetros: ponteiro para um arquivo, ponteiro para um header e valor booleano (true = escrever string semanticas, false = escrever somente struct)
Retorna:
    Caso bem-sucedido: true
    Caso contrário: false
*/
bool header_escrever(FILE* pontArq, HEADER* headerArq, bool semantico){
    fseek(pontArq, 0, SEEK_SET); // Posicionando pontArq no início do arquivo

    if(pontArq == NULL){
        printf("Erro com o ponteiro para o arquivo\n");
        return false;
    }

    // Escrevendo struct header no arquivo campo a campo
    fwrite(&(headerArq->status), sizeof(char), 1, pontArq);
    fwrite(&(headerArq->topo), sizeof(long int), 1, pontArq);
    fwrite(&(headerArq->proxByteOffset), sizeof(long int), 1, pontArq);
    fwrite(&(headerArq->nroRegArq), sizeof(int), 1, pontArq);
    fwrite(&(headerArq->nroRegRem), sizeof(int), 1, pontArq);

    // Escrevendo a parte semântica somente se necessário
    if(semantico){
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

        free(stringTemp); // Desalocando a string temporária
    }

    return true;
}

// TODO

DADO* dado_criar(char removido, int tamReg, long int prox, int idAttack, int year, float finLoss, char* country, char* attackType, char* targetInd, char* defMec){
        DADO *novoRegistro = (DADO *) malloc(sizeof(DADO));
        if (novoRegistro == NULL) {
            printf("Erro ao criar struct\n");

            return NULL;
        }

        // Inicializando struct

        // Campos de tamanho fixo
        novoRegistro->removido = removido;
        novoRegistro->tamanhoRegistro = tamReg;
        novoRegistro->prox = prox;
        novoRegistro->idAttack = idAttack;
        novoRegistro->year = year;
        novoRegistro->financialLoss = finLoss;


        // Campos de tamanho variável com delimitadores
        novoRegistro->country = formata_string_registro(country);
        novoRegistro->attackType = formata_string_registro(attackType);
        novoRegistro->targetIndustry = formata_string_registro(targetInd);
        novoRegistro->defenseMechanism = formata_string_registro(defMec);

        return novoRegistro;    // Retorna ponteiro para struct DADO
}

/* formata_string_registro():
Aloca dinamicamente memória para uma string e adiciona delimitadores no inicio e final
Parâmetros: string a ser formatada
Retorna: uma string formatada  
*/
char *formata_string_registro (char *string){
    if (string == NULL) {
        return NULL;
    }

    // Aloca memória para a string com o tamanho extra para os delimitadores
    char *strTemp = (char *) malloc(sizeof(char)*(strlen(string)+3));
    if (strTemp == NULL) {
        printf("ERRO : alocação mal sucedida");

        return NULL;
    }
    
    // Formatação da string com os delimitadores
    strcat(strTemp, "|");
    strcat(strTemp, string);
    strcat(strTemp, "|");

    return strTemp; // Retorna a string formatada
}

/*  set_dado_reg_tam():
Calcula o número de bytes do registro e atualiza na struct
Parâmetros: Ponteiro para struct
Retorna: booleano indicando status da operação
*/
bool set_dado_reg_tam (DADO *registro){
    if(registro == NULL){
        printf("Erro ao acessar registro\n");
        return false;
    }

    int contadorBytes = 25; // Inicializa o contador com o tamanho dos campos fixos
    
    // Adiciona ao contador o tamanho dos campos variáveis
    contadorBytes += strlen(registro->country);
    contadorBytes += strlen(registro->attackType);
    contadorBytes += strlen(registro->targetIndustry);
    contadorBytes += strlen(registro->defenseMechanism);
    
    registro->tamanhoRegistro = contadorBytes;

    return true;    
}
/*
bool set_prox_Byteoffset ();
calcula o numero de bytes para o proximo byte offset

retorna : tamanho registro + byteoffset atual
*/