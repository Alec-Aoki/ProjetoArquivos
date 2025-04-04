//
// Created by alecc on 4/4/25.
//

#include "arquivos.h"

/*Armazena os campos do header que sofrem alterações*/
typedef struct arquivo_{
    char status; // Consistência do arquivo de dados. 0 = inconsistente, 1 = consistente
    long int topo; // Byteoffset do primeiro registro logicamente removido. -1 = nenhum registro removido
    long int proxByteOffset; // Valor do próximo byteoffset disponível
    int nroRegArq; // Quantidade de registros não removidos
    int nroRegRem; // Quantidade de registros removidos
} ARQUIVO;