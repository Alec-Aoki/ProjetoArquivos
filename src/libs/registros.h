#ifndef REGISTROS_H
    #define REGISTROS_H

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

    typedef struct header_ HEADER;
    typedef struct dados_ DADO;

    // Aloca espaço na heap para um header e o inicializa
    HEADER* header_criar(void);

    // Desaloca a memória do header passado e aponta seu ponteiro para NULL
    void header_apagar(HEADER** header);

    // Escreve o header passado no arquivo binário
    bool header_escrever(char* nomeArquivo, HEADER* header);

    // TODO Aloca espaço na heap para um dado e o inicializa
    DADO* dado_criar(void);

    // TODO Escreve o dado passado no arquivo binário
    bool dado_escrever(char* nomeArquivo, DADO* dado);

#endif