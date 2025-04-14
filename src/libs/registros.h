#ifndef REGISTROS_H
    #define REGISTROS_H

    typedef struct header_ HEADER;
    typedef struct dados_ DADO;

    // Aloca espaço na heap para um header e o inicializa com as strings semânticas
    HEADER* header_criar(char* descIdent, char* descYear, char* descFinLoss, char* descCountry, char* descType, char* descTargInd, char* descDef);

    // Desaloca a memória do header passado e aponta seu ponteiro para NULL
    void header_apagar(HEADER** header);

    // Define o status de um header pré existente
    bool header_set_status(HEADER* header, char status);

    // Escreve o header passado no arquivo binário
    bool header_escrever(FILE* pontArq, HEADER* header, bool semantico);

#endif