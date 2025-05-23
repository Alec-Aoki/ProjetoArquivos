#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./registros/header.h"
#include "./registros/dados.h"

int main(void)
{
    // Criação do header com descrições
    HEADER *header = header_criar(
        "Identificador do Ataque",      // descIdent
        "Ano do Incidente",             // descYear
        "Perda Financeira (milhoes)",   // descFinLoss
        "Pais Afetado",                 // descCountry
        "Tipo de Ataque Cibernetico",   // descType
        "Industria Alvo",               // descTargInd
        "Mecanismo de Defesa Utilizado" // descDef
    );

    if (header == NULL)
    {
        printf("Erro na criação do header!\n");
        return 1;
    }

    // Configuração de valores do header
    header_set_status(header, '1');
    header_set_proxByteOffset(header, 1024);
    header_set_nroRegArq(header, 42);

    // Teste de header_get_nroRegArq
    printf("Registros no arquivo: %d\n", header_get_nroRegArq(header));

    // Criação de registros de dados
    DADO *dado1 = dado_criar(
        0,          // removido
        0,          // tamReg (será calculado)
        -1,         // prox
        1001,       // idAttack
        2023,       // year
        4.5,        // finLoss
        "Brasil",   // country
        "Phishing", // attackType
        "Saude",    // targetInd
        "Firewall"  // defMec
    );

    DADO *dado2 = dado_criar(
        1,             // removido
        0,             // tamReg (será calculado)
        512,           // prox
        -1,            // idAttack (inválido)
        -1,            // year (inválido)
        -1,            // finLoss (inválido)
        NULL,          // country (teste de NULL)
        "DDoS",        // attackType
        "",            // targetInd (vazio)
        "Criptografia" // defMec
    );

    printf("ALO VOCE\n");

    // Impressão dos registros
    printf("\n=== DADO 1 ===\n");
    dado_imprimir(header, dado1);

    printf("\n=== DADO 2 (campos invalidos) ===\n");
    dado_imprimir(header, dado2);

    // Limpeza de memória
    header_apagar(&header);
    dado_apagar(&dado1);
    dado_apagar(&dado2);

    return 0;
}