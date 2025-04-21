/*
Lida com a manipulação direta dos campos de structs
do tipo HEADER e DADOS, incluindo escrever e ler de/em
arquivos
*/

#ifndef REGISTROS_H
    #define REGISTROS_H

    // Tamanhos fixos dos campos do header (em bytes)
    #define TAM_DESC_ID 23
    #define TAM_DESC_YEAR 27
    #define TAM_DESC_FIN_LOSS 28
    #define TAM_DESC_COUNTRY 26
    #define TAM_DESC_TYPE 38
    #define TAM_DESC_TGT_IND 38
    #define TAM_DESC_DEF 67
    #define BYTEOFFSET_HEADER 276

    typedef struct header_ HEADER;
    typedef struct dados_ DADO;

    /* ------------------------------------------------------------------------------------- */
    /* FUNÇÕES DO HEADER*/
    /* ------------------------------------------------------------------------------------- */

    /* header_criar():
    Cria uma struct do tipo HEADER e a inicializa
    Parâmetros: ponteiros para strings (descrições do header)
    Retorna: ponteiro para a struct do tipo header
    */
    HEADER* header_criar(char* descIdent, char* descYear, char* descFinLoss, char* descCountry, char* descType, char* descTargInd, char* descDef);

    /* header_apagar():
    Desaloca uma struct do tipo header e define seu ponteiro para NULL
    Parâmetros: ponteiro de ponteiro para a struct a ser desalocada
    */
    void header_apagar(HEADER** header);

    /* header_escrever():
    Escreve um header passado no arquivo binário
    Parâmetros: ponteiro para um arquivo, ponteiro para um header e valor booleano (true = escrever string semanticas, false = escrever somente struct)
    */
    void header_escrever(FILE* pontArq, HEADER* header, bool semantico);

    /* header_ler():
    Lê os campos do header de um arquivo binário e guarda em uma struct do tipo HEADER
    Parâmetros: ponteiro para arquivo, ponteiro para header
    Retorna: ponteiro para header
    */
    HEADER* header_ler(FILE* pontArq, HEADER* header);

    /* header_set_status():
    Define o campo de status de um header pré-existente
    Parâmetros: ponteiro para o header, status a ser definido
    */
    void header_set_status(HEADER* header, char status);

    /* header_set_proxByteOffset()
    Define o campo proxByteOffset de um header
    Parâmetros: ponteiro para header, valor do próximo byte offset livre
    */
    void header_set_proxByteOffset(HEADER* header, long int proxByOff);

    /* header_set_nroRegArq()
    Define o campo nroRegArq de um header
    Parâmetros: ponteiro para header, quantidade de registros no arquivos
    Retorno: false se header nulo, true caso contrário
    */
    void header_set_nroRegArq(HEADER* header, int nroRegAq);

    /* header_get_nroRegArq
    Retorna o valor do campo nroRegArq de uma struct header
    Parâmetros: ponteiro para struct do tipo header
    Retorna: valor do campo nroRegArq da struct (-1 se header == NULL)
    */
    int header_get_nroRegArq(HEADER* header);

    /* ------------------------------------------------------------------------------------- */
    /* FUNÇÕES DOS DADOS */
    /* ------------------------------------------------------------------------------------- */

    /* dado_criar():
    Aloca memória para uma struct do tipo dado e inicializa seus campos
    Parâmetros: valores dos campos da struct
    Retorna: ponteiro para dado
    */
    DADO* dado_criar(char removido, int tamReg, long int prox, int idAttack, int year, float finLoss, char* country, char* attackType, char* targetInd, char* defMec);

    /* dado_apagar():
    Desaloca memória da struct e dos campos de tamanho variável
    Parâmetro: ponteiro para ponteiro da struct
    */
    void dado_apagar(DADO **registro);

    /*dado_escrever():
    Escreve os campos de uma struct dado em um arquivo
    Parâmetros: ponteiro para arquivo, ponteiro para uma struct dado
    */
    void dado_escrever (FILE *pontArqBin, DADO *dado);

    /* dado_ler():
    Lê um registro do arquivo e guarda numa struct DADO
    Parâmetros: Ponteiro para arquivo, ponteiro para struct DADO, byteOffset do registro
    Retorna: Ponteiro para struct DADO
    */
    DADO* dado_ler(FILE* pontArq, DADO* dado, int byteOffset);

    /* dado_imprimir():
    Imprime um dado usando as descrições semânticas do header
    Parâmetros: ponteiro para o header, ponteiro para o dado
    */
    void dado_imprimir(HEADER* header, DADO* dado);

    /* dado_get_tamanho()
    Retorna o tamanho em bytes de um registro de dado
    Parâmetros: ponteiro para struct dado
    Retorno: -1 caso a struct seja nula, caso contrário o valor guardado no campo tamanhoRegistro
    */
    int dado_get_tamanho(DADO* dado);


    /* dado_get_idAttack()
    Retorna o valor do campo idAttack de uma struct dado
    Parâmetros: ponteiro para struct dado
    Retorno: -1 caso a struct seja nula, caso contrário o valor guardado no campo idAttack
    */
    int dado_get_idAttacK(DADO* dado);

    /* dado_get_year()
    Retorna o valor do campo year de uma struct dado
    Parâmetros: ponteiro para struct dado
    Retorno: -1 caso a struct seja nula, caso contrário o valor guardado no campo year
    */
    int dado_get_year(DADO* dado);

    /* dado_get_finLoss()
    Retorna o valor do campo financialLoss de uma struct dado
    Parâmetros: ponteiro para struct dado
    Retorno: -1 caso a struct seja nula, caso contrário o valor guardado no campo financialLoss
    */
    float dado_get_finLoss(DADO* dado);

    /* dado_get_country()
    Retorna a string guardada no campo country
    Parâmetros: ponteiro para struct dado
    Retorno: NULL caso a struct seja nula, caso contrário um ponteio para a string guardada no campo country
    */
    char* dado_get_country(DADO* dado);

    /* dado_get_attackType()
    Retorna a string guardada no campo attackTyoe
    Parâmetros: ponteiro para struct dado
    Retorno: NULL caso a struct seja nula, caso contrário um ponteio para a string guardada no campo attackType
    */
    char* dado_get_attackType(DADO* dado);

    /* dado_get_targetIndustry()
    Retorna a string guardada no campo targetIndustry
    Parâmetros: ponteiro para struct dado
    Retorno: NULL caso a struct seja nula, caso contrário um ponteio para a string guardada no campo targetIndustry
    */
    char* dado_get_targetIndustry(DADO* dado);

    /* dado_get_defenseMech()
    Retorna a string guardada no campo defenseMechanism
    Parâmetros: ponteiro para struct dado
    Retorno: NULL caso a struct seja nula, caso contrário um ponteio para a string guardada no campo defenseMechanism
    */
    char* dado_get_defenseMech(DADO* dado);
#endif