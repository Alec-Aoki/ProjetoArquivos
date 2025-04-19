#ifndef REGISTROS_H
    #define REGISTROS_H

    // Tamanho das strings do header (em bytes)
    #define TAM_DESC_ID 23
    #define TAM_DESC_YEAR 27
    #define TAM_DESC_FIN_LOSS 28
    #define TAM_DESC_COUNTRY 26
    #define TAM_DESC_TYPE 38
    #define TAM_DESC_TGT_IND 38
    #define TAM_DESC_DEF 67

    typedef struct header_ HEADER;
    typedef struct dados_ DADO;

    /* ------------------------------------------------------------------------------------- */
    /* FUNÇÕES DO HEADER*/
    /* ------------------------------------------------------------------------------------- */

    // Aloca espaço na heap para um header e o inicializa com as strings semânticas
    HEADER* header_criar(char* descIdent, char* descYear, char* descFinLoss, char* descCountry, char* descType, char* descTargInd, char* descDef);

    // Desaloca a memória do header passado e aponta seu ponteiro para NULL
    void header_apagar(HEADER** header);

    // Escreve o header passado no arquivo binário
    void header_escrever(FILE* pontArq, HEADER* header, bool semantico);

    // Lê o header de um arquivo binário
    HEADER* header_ler(FILE* pontArq, HEADER* header);

    // Define o status de um header pré existente
    void header_set_status(HEADER* header, char status);

    // Define o campo próximo byteOffset de um header
    void header_set_proxByteOffset(HEADER* header, long int proxByOff);

    // Define o campo nroRegArq de um header
    void header_set_nroRegArq(HEADER* header, int nroRegAq);

    // Retorna o campo nroRegArq de um header
    int header_get_nroRegArq(HEADER* header);

    /* ------------------------------------------------------------------------------------- */
    /* FUNÇÕES DOS DADOS */
    /* ------------------------------------------------------------------------------------- */

    // Aloca espaço na heap para um dado e o inicializa com os valores passados
    DADO* dado_criar(char removido, int tamReg, long int prox, int idAttack, int year, float finLoss, char* country, char* attackType, char* targetInd, char* defMec);

    // Desaloca a memória do dado passado e aponta seu ponteiro para NULL
    void dado_apagar(DADO **registro);

    // Escerve o dado passado no arquivo binário
    void dado_escrever (FILE *pontArqBin, DADO *dado);

    // Lê um dado de um arquivo binário
    DADO* dado_ler(FILE* pontArq, DADO* dado, int byteOffset);

    // Imprime os campos de dado de acordo com as descrições do header
    void dado_imprimir(HEADER* header, DADO* dado);

    // Retorna o valor do campo tamanhoRegistro de um dado
    int dado_get_tamanho(DADO* dado);

    int dado_get_idAttacK(DADO* dado);

    int dado_get_year(DADO* dado);

    float dado_get_finLoss(DADO* dado);

    char* dado_get_country(DADO* dado);

    char* dado_get_attackType(DADO* dado);

    char* dado_get_targetIndustry(DADO* dado);

    char* dado_get_defenseMech(DADO* dado);
#endif