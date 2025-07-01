#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../registros/header.h"
#include "../../registros/dados.h"
#include "../../auxiliar/auxiliar.h"
#include "../../registros/busca.h"
#include "../arvB.h"

struct header_arvB_
{
    int status;  // 0 = inconsistente, 1 = consistente
    int noRaiz;  // byteOffset
    int proxRRN; // byteOffset
    int nroNos;
};

struct no_
{
    int byteOffset;                             // byteOffset atual do reg.
    int chaves[quantMaxChaves];                 // idAttacks
    int byteOffsetDados[quantMaxChaves];        // byteOffsets no arq. de dados
    int byteOffsetDescendentes[quantMaxFilhos]; // byteOffsets dos nós filhos
    int tipoNo;                                 // -1 = folha, 0 = raiz, 1 = interm.
    int quantChavesAtual;
};