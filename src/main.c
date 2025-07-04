#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

// #include "./libs/arquivos/funcionalidades.h"

#include "./libs/arquivos/arvB.h"

// Função principal
int main()
{
    printf("\n=== TESTE DE INSERÇÃO ===\n");

    // Teste 1: Primeira inserção (árvore vazia)
    FILE *arquivo = fopen("teste_insercao.bin", "wb+");
    if (arquivo == NULL)
    {
        printf("ERRO: Falha ao criar arquivo para inserção\n");
        return 0;
    }

    HEADER_ARVB *header = ArvB_header_criar();
    ArvB_header_escrever(arquivo, header);

    printf("1. Inserindo chave 10 (árvore vazia)...\n");
    // Inserir primeira chave
    ArvB_inserir(arquivo, header, 10, 1000);

    // Verificar se a inserção foi bem-sucedida
    int noRaiz = ArvB_header_get_int(header, 1);
    int nroNos = ArvB_header_get_int(header, 3);

    printf("   noRaiz após inserção: %d (esperado: %d)\n", noRaiz, TAM_HEADER_ARVB);
    printf("   nroNos após inserção: %d (esperado: 1)\n", nroNos);

    if (noRaiz != TAM_HEADER_ARVB || nroNos != 1)
    {
        printf("ERRO: Primeira inserção falhou\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        return 0;
    }

    NO *noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB);
    print_no(noLeitura);

    // Teste 2: Inserção adicional (nó folha com espaço)
    printf("2. Inserindo chave 5 (nó folha com espaço)...\n");
    ArvB_inserir(arquivo, header, 5, 500);

    // Verificar se ainda temos apenas um nó
    nroNos = ArvB_header_get_int(header, 3);
    printf("   nroNos após segunda inserção: %d (esperado: 1)\n", nroNos);

    if (nroNos != 1)
    {
        printf("ERRO: Segunda inserção causou split desnecessário\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        return 0;
    }

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB);
    print_no(noLeitura);

    // Teste 3: Inserção que causa split
    printf("3. Inserindo chave 15 (split)...\n");
    ArvB_inserir(arquivo, header, 15, 1500);

    // Verificar se houve split (devemos ter mais nós)
    nroNos = ArvB_header_get_int(header, 3);
    printf("   nroNos após terceira inserção: %d (esperado: 3)\n", nroNos);

    if (nroNos != 3)
    {
        printf("ERRO: Split não ocorreu corretamente\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        return 0;
    }

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB);
    print_no(noLeitura);

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB + TAM_REGISTRO_ARVB);
    print_no(noLeitura);

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB + 2 * TAM_REGISTRO_ARVB);
    print_no(noLeitura);

    // Teste 4: Inserções adicionais
    printf("4. Inserindo chave 20 (nó folha com espaço)...\n");
    ArvB_inserir(arquivo, header, 20, 2000);

    nroNos = ArvB_header_get_int(header, 3);
    printf("   nroNos após terceira inserção: %d (esperado: 3)\n", nroNos);

    if (nroNos != 3)
    {
        printf("ERRO: Houve split desnecessario\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        return 0;
    }

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB);
    print_no(noLeitura);

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB + TAM_REGISTRO_ARVB);
    print_no(noLeitura);

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB + 2 * TAM_REGISTRO_ARVB);
    print_no(noLeitura);

    printf("4. Inserindo chave 25 (split)...\n");
    ArvB_inserir(arquivo, header, 25, 2500);

    nroNos = ArvB_header_get_int(header, 3);
    printf("   nroNos após terceira inserção: %d (esperado: 4)\n", nroNos);

    if (nroNos != 4)
    {
        printf("ERRO: Split não ocorreu corretamente\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        return 0;
    }

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB);
    print_no(noLeitura);

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB + TAM_REGISTRO_ARVB);
    print_no(noLeitura);

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB + 2 * TAM_REGISTRO_ARVB);
    print_no(noLeitura);

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB + 3 * TAM_REGISTRO_ARVB);
    print_no(noLeitura);

    printf("5. Inserindo chave 30 (nó folha com espaço)...\n");
    ArvB_inserir(arquivo, header, 30, 3000);

    nroNos = ArvB_header_get_int(header, 3);
    printf("   nroNos após terceira inserção: %d (esperado: 4)\n", nroNos);

    if (nroNos != 4)
    {
        printf("ERRO: Houve split desnecessário\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        return 0;
    }

    print_no(noLeitura);

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB + TAM_REGISTRO_ARVB);
    print_no(noLeitura);

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB + 2 * TAM_REGISTRO_ARVB);
    print_no(noLeitura);

    noLeitura = ArvB_no_ler(arquivo, TAM_HEADER_ARVB + 3 * TAM_REGISTRO_ARVB);
    print_no(noLeitura);

    printf("6. Inserindo chave 35 (split)...\n");
    ArvB_inserir(arquivo, header, 35, 3500);

    nroNos = ArvB_header_get_int(header, 3);
    printf("   nroNos após terceira inserção: %d (esperado: 7)\n", nroNos);

    if (nroNos != 7)
    {
        printf("ERRO: Houve splits incorretos\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        return 0;
    }

    printf("7. Inserindo chave 40 (nó folha com espaço)...\n");
    ArvB_inserir(arquivo, header, 40, 4000);

    nroNos = ArvB_header_get_int(header, 3);
    printf("   nroNos após terceira inserção: %d (esperado: 7)\n", nroNos);

    if (nroNos != 7)
    {
        printf("ERRO: Houve split desnecessário\n");
        fclose(arquivo);
        ArvB_header_apagar(&header);
        return 0;
    }

    printf("\n=== TESTE DE BUSCA ===\n");

    int chavesFinal[] = {10, 5, 15, 20, 25, 30, 35, 40};

    // Teste 2: Buscar chaves existentes
    printf("2. Testando busca de chaves existentes...\n");
    noRaiz = ArvB_header_get_int(header, 1);

    for (int i = 0; i < 8; i++)
    {
        NO *resultado = ArvB_busca(arquivo, noRaiz, chavesFinal[i]);
        if (resultado == NULL)
        {
            printf("ERRO: Chave %d não encontrada\n", chavesFinal[i]);
            fclose(arquivo);
            ArvB_header_apagar(&header);
            return 0;
        }
        printf("   ✓ Chave %d encontrada\n", chavesFinal[i]);
        ArvB_no_apagar(&resultado);
    }

    // Limpeza
    fclose(arquivo);
    ArvB_header_apagar(&header);
    remove("teste_busca.bin");

    return 0;
}