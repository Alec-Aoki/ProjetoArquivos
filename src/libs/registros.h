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

    // Aloca espaço na heap para um header e o inicializa com as strings semânticas
    HEADER* header_criar(char* descIdent, char* descYear, char* descFinLoss, char* descCountry, char* descType, char* descTargInd, char* descDef);

    // Desaloca a memória do header passado e aponta seu ponteiro para NULL
    void header_apagar(HEADER** header);

    // Define o status de um header pré existente
    void header_set_status(HEADER* header, char status);

    // Define o campo próximo byteOffset de um header
    void header_set_proxByteOffset(HEADER* header, long int proxByOff);

    // Define o campo nroRegArq de um header
    void header_set_nroRegArq(HEADER* header, int nroRegAq);

    // Escreve o header passado no arquivo binário
    void header_escrever(FILE* pontArq, HEADER* header, bool semantico);

    // Aloca espaço na heap para um dado e o inicializa com os valores passados
    DADO* dado_criar(char removido, int tamReg, long int prox, int idAttack, int year, float finLoss, char* country, char* attackType, char* targetInd, char* defMec);

    // Retorna o valor do campo tamanhoRegistro de um dado
    int dado_get_tamanho(DADO* dado);

    // Escrve o dado passado no arquivo binário
    void dado_escrever (FILE *pontArqBin, DADO *dado);

    // Desaloca a memória do dado passado e aponta seu ponteiro para NULL
    void dado_apagar(DADO **registro);
#endif