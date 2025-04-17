#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "registros.h"

/*Armazena os campos de um registro de header*/
struct header_{
    char status; // Consistência do arquivo de dados. 0 = inconsistente, 1 = consistente
    long int topo; // Byteoffset do primeiro registro logicamente removido. -1 = nenhum registro removido
    long int proxByteOffset; // Valor do próximo byteoffset disponível
    int nroRegArq; // Quantidade de registros não removidos
    int nroRegRem; // Quantidade de registros removidos

    char descreveIdentificador[TAM_DESC_ID]; // Descrição do campo idAttack
    char descreveYear[TAM_DESC_YEAR]; // Descrição do campo year
    char descreveFinancialLoss[TAM_DESC_FIN_LOSS]; // Descrição do campo financialLoss
    char codDescreveCountry; // Código da keyword que representa o campo country
    char descreveCountry[TAM_DESC_COUNTRY]; // Descrição do campo country
    char codDescreveType; // Código da keyword que representa o campo attackType
    char descreveType[TAM_DESC_TYPE]; // Descrição do campo attackType
    char codDescreveTargetIndustry; // Código da keyword que representa o campo targetIndustry
    char descreveTargetIndustry[TAM_DESC_TGT_IND]; // Descrição do campo targetIndustry
    char codDescreveDefense; // Código da keyword que representa o campo defenseMechanism
    char descreveDefense[TAM_DESC_DEF]; // Descrição do campo defenseMechanism
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
Parâmetros: ponteiros para strings (descrições do header)
Retorna: ponteiro para a struct do tipo HEADER
*/
HEADER* header_criar(char* descIdent, char* descYear, char* descFinLoss, char* descCountry, char* descType, char* descTargInd, char* descDef){
    HEADER* novoHeader = (HEADER *) malloc(sizeof(HEADER)); // Alocando dinâmicamente uma struct do tipo HEADER
    if(novoHeader == NULL){
        printf("Erro ao criar struct\n");

        return NULL;
    }

    /* INICIALIZANDO O HEADER */
    // Campos de valor variável
    novoHeader->status = '0';
    novoHeader->topo = -1;
    novoHeader->proxByteOffset = 0;
    novoHeader->nroRegArq = 0;
    novoHeader->nroRegRem = 0;

    // Campos de valor fixo (semânticos)
    novoHeader->codDescreveCountry = '1';
    novoHeader->codDescreveType = '2';
    novoHeader->codDescreveTargetIndustry = '3';
    novoHeader->codDescreveDefense = '4';
    
    strncpy(novoHeader->descreveIdentificador, descIdent, TAM_DESC_ID);
    strncpy(novoHeader->descreveYear, descYear, TAM_DESC_YEAR);
    strncpy(novoHeader->descreveFinancialLoss, descFinLoss, TAM_DESC_FIN_LOSS);
    strncpy(novoHeader->descreveCountry, descCountry, TAM_DESC_COUNTRY);
    strncpy(novoHeader->descreveType, descType, TAM_DESC_TYPE);
    strncpy(novoHeader->descreveTargetIndustry, descTargInd, TAM_DESC_TGT_IND);
    strncpy(novoHeader->descreveDefense, descDef, TAM_DESC_DEF);

    return novoHeader; // Retornando ponteiro para HEADER
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

    // Escrevendo os campos variáveis da struct header no arquivo
    fwrite(&(headerArq->status), sizeof(char), 1, pontArq);
    fwrite(&(headerArq->topo), sizeof(long int), 1, pontArq);
    fwrite(&(headerArq->proxByteOffset), sizeof(long int), 1, pontArq);
    fwrite(&(headerArq->nroRegArq), sizeof(int), 1, pontArq);
    fwrite(&(headerArq->nroRegRem), sizeof(int), 1, pontArq);

    // Escrevendo a parte semântica somente se necessário
    if(semantico){        
        // descreveIdentificador: descrição do campo idAttack
        fwrite(headerArq->descreveIdentificador, sizeof(char), TAM_DESC_ID, pontArq);

        // descreveYear: descrição do campo year
        fwrite(headerArq->descreveYear, sizeof(char), TAM_DESC_YEAR, pontArq);

        // descreveFinancialLoss: descrição do campo financialLoss
        fwrite(headerArq->descreveFinancialLoss, sizeof(char), TAM_DESC_FIN_LOSS, pontArq);

        // codDescreveCountry: código da keyword que representa o campo country
        fwrite(&(headerArq->codDescreveCountry), sizeof(char), 1, pontArq);

        // descreveYear: descrição do campo country
        fwrite(headerArq->descreveCountry, sizeof(char), TAM_DESC_COUNTRY, pontArq);

        // codDescreveType: código da keyword que representa o campo type
        fwrite(&(headerArq->codDescreveType), sizeof(char), 1, pontArq);

        // descreveType: descrição do campo type
        fwrite(headerArq->descreveType, sizeof(char), TAM_DESC_TYPE, pontArq);

        // codDescreveTargetIndustry: código da keyword que representa o campo targetIndustry
        fwrite(&(headerArq->codDescreveTargetIndustry), sizeof(char), 1, pontArq);

        // descreveTargetIndustry: descrição do campo targetIndustry
        fwrite(headerArq->descreveTargetIndustry, sizeof(char), TAM_DESC_TGT_IND, pontArq);

        // codDescreveDefense: código da keyword que representa o campo defenseMechanism
        fwrite(&(headerArq->codDescreveDefense), sizeof(char), 1, pontArq);

        // descreveDefense: descrição do campo defenseMechanism
        fwrite(headerArq->descreveDefense, sizeof(char), TAM_DESC_DEF, pontArq);
    }

    return true;
}
// Funções auxiliares, explicadas mais adiante
bool dado_set_tamReg (DADO *registro);
char *formata_string_registro (char *string, char *id);

DADO* dado_criar(char removido, int tamReg, long int prox, int idAttack, int year, float finLoss, char* country, char* attackType, char* targetInd, char* defMec){
    DADO *novoRegistro = (DADO *) malloc(sizeof(DADO));
    if (novoRegistro == NULL) {
        printf("Erro ao criar struct\n");
        return NULL;
    }

    /* INICIALIZANDO A STRUCT */

    // Campos de tamanho fixo
    novoRegistro->removido = removido;
    novoRegistro->tamanhoRegistro = tamReg;
    novoRegistro->prox = prox;
    novoRegistro->idAttack = idAttack;
    novoRegistro->year = year;
    novoRegistro->financialLoss = finLoss;


    // Campos de tamanho variável com delimitadores
    novoRegistro->country = formata_string_registro(country, "1");
    novoRegistro->attackType = formata_string_registro(attackType, "2");
    novoRegistro->targetIndustry = formata_string_registro(targetInd, "3");
    novoRegistro->defenseMechanism = formata_string_registro(defMec, "4");

    dado_set_tamReg(novoRegistro);

    return novoRegistro; // Retorna ponteiro para struct DADO
}

bool dado_escrever (FILE *pontArqBin, DADO *dado){
        // Verifiva a corretude dos ponteiros
        if (pontArqBin == NULL) {
            printf("Erro com o ponteiro para o nome do arquivo\n");
            return false;
        }
    
        if (dado == NULL) {
            printf("Erro de struct nula\n");
            return false;
        }
    
        // Escreve os dados no arquivo binário
        fwrite(&(dado->removido), sizeof(int), 1, pontArqBin);
        fwrite(&(dado->tamanhoRegistro), sizeof(int), 1, pontArqBin);
        fwrite(&(dado->prox), sizeof(long int), 1, pontArqBin);
        fwrite(&(dado->idAttack), sizeof(int), 1, pontArqBin);
        fwrite(&(dado->year), sizeof(int), 1, pontArqBin);
        fwrite(&(dado->financialLoss), sizeof(float), 1, pontArqBin);

        if(dado->country != NULL) fwrite(dado->country, sizeof(char), strlen(dado->country), pontArqBin);
        if(dado->attackType != NULL)fwrite(dado->attackType, sizeof(char), strlen(dado->attackType), pontArqBin);
        if(dado->targetIndustry != NULL) fwrite(dado->targetIndustry, sizeof(char), strlen(dado->targetIndustry), pontArqBin);
        if(dado->defenseMechanism != NULL) fwrite(dado->defenseMechanism, sizeof(char), strlen(dado->defenseMechanism), pontArqBin);
    
        // Retorna o status da operação
        return true;
}

/* dado_set_tamReg():
Calcula o número de bytes do registro e atualiza na struct
Parâmetros: Ponteiro para struct
Retorna: booleano indicando status da operação
*/
bool dado_set_tamReg (DADO *registro){
    if(registro == NULL){
        printf("Erro ao acessar registro\n");
        return false;
    }

    int contadorBytes = 0; // Inicializa o contador com o tamanho dos campos fixos
    
    // Adiciona ao contador o tamanho dos campos fixos
    contadorBytes += sizeof(registro->removido);
    contadorBytes += sizeof(registro->tamanhoRegistro);
    contadorBytes += sizeof(registro->prox);
    contadorBytes += sizeof(registro->idAttack);
    contadorBytes += sizeof(registro->year);
    contadorBytes += sizeof(registro->financialLoss);
    
    // Adiciona ao contador o tamanho dos campos variáveis
    if (registro->country != NULL) contadorBytes += strlen(registro->country);
    if (registro->attackType != NULL) contadorBytes += strlen(registro->attackType);
    if (registro->targetIndustry != NULL) contadorBytes += strlen(registro->targetIndustry);
    if (registro->defenseMechanism != NULL) contadorBytes += strlen(registro->defenseMechanism);
    
    registro->tamanhoRegistro = contadorBytes;

    return true;  
}

/* formata_string_registro():
Aloca dinamicamente memória para uma string e adiciona delimitadores no inicio e final
Parâmetros: string a ser formatada
Retorna: uma string formatada  
*/
char *formata_string_registro (char *string, char *id){
    if (string == NULL || strcmp(string, "") == 0) {
        return "|";
    }

    // Aloca memória para a string com o tamanho extra para os delimitadores
    char *strTemp = (char *) malloc(sizeof(char)*(strlen(string)+strlen(id)+2));
    if (strTemp == NULL) {
        printf("ERRO : alocação mal sucedida");

        return NULL;
    }
    
    // Construção segura da string
    strTemp[0] = '\0'; // Inicializa o buffer
    strcat(strTemp, id);
    strcat(strTemp, string);
    strcat(strTemp, "|");

    return strTemp; // Retorna a string formatada
}
