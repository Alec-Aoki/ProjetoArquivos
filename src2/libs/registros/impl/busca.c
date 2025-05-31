#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../auxiliar/auxiliar.h"
#include "../header.h"
#include "../dados.h"
#include "../busca.h"

/*Armazena campos de uma busca*/
struct busca_
{

    int quantCampos;    // Quantidade de campos que serão pesquisados
    int quaisCampos[7]; // Quant. máx. de campos = 7
    int idAttack;
    int year;
    float finLoss;
    char country[TAM_MAX_STR];
    char attackType[TAM_MAX_STR];
    char targetIndustry[TAM_MAX_STR];
    char defenseMechanism[TAM_MAX_STR];
};

BUSCA *buscaCriar();

void busca_apagar(BUSCA **busca);