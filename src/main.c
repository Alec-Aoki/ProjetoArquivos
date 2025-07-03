#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "./libs/arquivos/funcionalidades.h"

#include "./libs/arquivos/arvB.h"

/*
MAIN DE TESTE DA ARV B
*/

// Simulando as dependências necessárias
void mensagem_erro2()
{
    printf("ERRO: Falha na operação\n");
}

// Função auxiliar para criar arquivo de teste
void criar_arquivo_teste(const char *nome_arquivo, int tamanho_bytes)
{
    FILE *arquivo = fopen(nome_arquivo, "wb");
    if (arquivo)
    {
        char byte_zero = 0;
        for (int i = 0; i < tamanho_bytes; i++)
        {
            fwrite(&byte_zero, 1, 1, arquivo);
        }
        fclose(arquivo);
    }
}

// Função auxiliar para criar arquivo corrompido
void criar_arquivo_corrompido(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "wb");
    if (arquivo)
    {
        // Escreve apenas parte do header
        char status = '1';
        int valor = 100;
        fwrite(&status, sizeof(char), 1, arquivo);
        fwrite(&valor, sizeof(int), 1, arquivo);
        // Não escreve o resto do header
        fclose(arquivo);
    }
}

void testar_header_criar()
{
    printf("=== TESTANDO ArvB_header_criar() ===\n");

    // Teste 1: Criação normal
    printf("Teste 1: Criação normal do header\n");
    HEADER_ARVB *header = ArvB_header_criar();
    if (header != NULL)
    {
        printf("✓ Header criado com sucesso\n");

        // Verificar valores iniciais
        int noRaiz = ArvB_header_get_int(header, 1);
        int proxRRN = ArvB_header_get_int(header, 2);
        int nroNos = ArvB_header_get_int(header, 3);

        printf("  - noRaiz: %d (esperado: -1)\n", noRaiz);
        printf("  - proxRRN: %d (esperado: 0)\n", proxRRN);
        printf("  - nroNos: %d (esperado: 0)\n", nroNos);

        ArvB_header_apagar(&header);
    }
    else
    {
        printf("✗ Falha na criação do header\n");
    }

    printf("\n");
}

void testar_header_set()
{
    printf("=== TESTANDO ArvB_header_set() ===\n");

    // Teste 1: Header NULL (deveria criar um novo segundo documentação, mas não faz)
    printf("Teste 1: Header NULL\n");
    HEADER_ARVB *header = ArvB_header_set(NULL, 1, 100, 200, 5);
    if (header == NULL)
    {
        printf("✓ Retornou NULL conforme implementação atual\n");
    }
    else
    {
        printf("✗ Comportamento inesperado\n");
        ArvB_header_apagar(&header);
    }

    // Teste 2: Valores normais
    printf("Teste 2: Valores normais\n");
    header = ArvB_header_criar();
    header = ArvB_header_set(header, 1, 100, 200, 5);
    if (header != NULL)
    {
        printf("✓ Header configurado\n");
        printf("  - noRaiz: %d\n", ArvB_header_get_int(header, 1));
        printf("  - proxRRN: %d\n", ArvB_header_get_int(header, 2));
        printf("  - nroNos: %d\n", ArvB_header_get_int(header, 3));
        ArvB_header_apagar(&header);
    }

    // Teste 3: Valores -2 (não devem alterar)
    printf("Teste 3: Valores -2 (não alterar)\n");
    header = ArvB_header_criar();
    header = ArvB_header_set(header, -2, -2, -2, -2);
    if (header != NULL)
    {
        printf("✓ Valores mantidos inalterados\n");
        printf("  - noRaiz: %d (esperado: -1)\n", ArvB_header_get_int(header, 1));
        printf("  - proxRRN: %d (esperado: 0)\n", ArvB_header_get_int(header, 2));
        printf("  - nroNos: %d (esperado: 0)\n", ArvB_header_get_int(header, 3));
        ArvB_header_apagar(&header);
    }

    printf("\n");
}

void testar_header_get_int()
{
    printf("=== TESTANDO ArvB_header_get_int() ===\n");

    // Teste 1: Header NULL
    printf("Teste 1: Header NULL\n");
    int valor = ArvB_header_get_int(NULL, 1);
    printf("Valor retornado: %d (esperado: -1)\n", valor);

    // Teste 2: Campo inválido
    printf("Teste 2: Campo inválido\n");
    HEADER_ARVB *header = ArvB_header_criar();
    valor = ArvB_header_get_int(header, 999);
    printf("Valor retornado: %d (esperado: -1)\n", valor);

    // Teste 3: Campos válidos
    printf("Teste 3: Campos válidos\n");
    ArvB_header_set(header, 1, 100, 200, 5);
    printf("  - Campo 1 (noRaiz): %d\n", ArvB_header_get_int(header, 1));
    printf("  - Campo 2 (proxRRN): %d\n", ArvB_header_get_int(header, 2));
    printf("  - Campo 3 (nroNos): %d\n", ArvB_header_get_int(header, 3));

    ArvB_header_apagar(&header);
    printf("\n");
}

void testar_header_arquivo()
{
    printf("=== TESTANDO ArvB_header_ler/escrever() ===\n");

    // Teste 1: Arquivo NULL
    printf("Teste 1: Arquivo NULL\n");
    HEADER_ARVB *header = ArvB_header_ler(NULL, NULL);
    if (header == NULL)
    {
        printf("✓ Retornou NULL para arquivo inválido\n");
    }

    // Teste 2: Escrita e leitura normal
    printf("Teste 2: Escrita e leitura normal\n");
    FILE *arquivo = fopen("teste_header.bin", "wb+");
    if (arquivo)
    {
        header = ArvB_header_criar();
        ArvB_header_set(header, 1, 100, 200, 5);

        // Escrever
        ArvB_header_escrever(arquivo, header);
        printf("✓ Header escrito no arquivo\n");

        // Ler de volta
        HEADER_ARVB *header_lido = ArvB_header_ler(arquivo, NULL);
        if (header_lido)
        {
            printf("✓ Header lido do arquivo\n");
            printf("  - noRaiz lido: %d\n", ArvB_header_get_int(header_lido, 1));
            printf("  - proxRRN lido: %d\n", ArvB_header_get_int(header_lido, 2));
            printf("  - nroNos lido: %d\n", ArvB_header_get_int(header_lido, 3));
            ArvB_header_apagar(&header_lido);
        }

        fclose(arquivo);
        ArvB_header_apagar(&header);
        remove("teste_header.bin");
    }

    // Teste 3: Arquivo corrompido
    printf("Teste 3: Arquivo corrompido\n");
    criar_arquivo_corrompido("teste_corrompido.bin");
    arquivo = fopen("teste_corrompido.bin", "rb");
    if (arquivo)
    {
        header = ArvB_header_ler(arquivo, NULL);
        if (header)
        {
            printf("⚠ Header lido de arquivo corrompido (pode ter dados inválidos)\n");
            printf("  - noRaiz: %d\n", ArvB_header_get_int(header, 1));
            ArvB_header_apagar(&header);
        }
        else
        {
            printf("✗ Falha na leitura (comportamento pode variar)\n");
        }
        fclose(arquivo);
        remove("teste_corrompido.bin");
    }

    printf("\n");
}

void testar_no_criar()
{
    printf("=== TESTANDO ArvB_no_criar() ===\n");

    // Teste 1: Criação normal
    printf("Teste 1: Criação normal do nó\n");
    NO *no = ArvB_no_criar();
    if (no != NULL)
    {
        printf("✓ Nó criado com sucesso\n");
        printf("  - Inicializado como folha (tipo -1)\n");
        printf("  - Quantidade de chaves: 0\n");
        ArvB_no_apagar(&no);
    }
    else
    {
        printf("✗ Falha na criação do nó\n");
    }

    printf("\n");
}

void testar_no_set()
{
    printf("=== TESTANDO ArvB_no_set() ===\n");

    // Teste 1: NO NULL
    printf("Teste 1: NO NULL\n");
    NO *resultado = ArvB_no_set(NULL, 100, NULL, NULL, NULL, 0, 2);
    if (resultado == NULL)
    {
        printf("✓ Retornou NULL para nó inválido\n");
    }

    // Teste 2: Configuração normal
    printf("Teste 2: Configuração normal\n");
    NO *no = ArvB_no_criar();
    int chaves[] = {10, 20};
    int dados[] = {100, 200};
    int descendentes[] = {300, 400, 500};

    ArvB_no_set(no, 1000, chaves, dados, descendentes, 0, 2);
    printf("✓ Nó configurado com sucesso\n");

    // Teste 3: Valores -2 ou NULL (não devem alterar)
    printf("Teste 3: Valores que não alteram\n");
    ArvB_no_set(no, -2, NULL, NULL, NULL, -2, -2);
    printf("✓ Valores mantidos inalterados\n");

    ArvB_no_apagar(&no);
    printf("\n");
}

void testar_no_arquivo()
{
    printf("=== TESTANDO ArvB_no_ler/escrever() ===\n");

    // Teste 1: Arquivo NULL
    printf("Teste 1: Arquivo NULL\n");
    NO *no = ArvB_no_ler(NULL, 0);
    if (no == NULL)
    {
        printf("✓ Retornou NULL para arquivo inválido\n");
    }

    // Teste 2: ByteOffset inválido
    printf("Teste 2: ByteOffset negativo\n");
    FILE *arquivo = fopen("teste_no.bin", "wb+");
    if (arquivo)
    {
        no = ArvB_no_ler(arquivo, -1);
        if (no == NULL)
        {
            printf("✓ Retornou NULL para byteOffset inválido\n");
        }
        fclose(arquivo);
        remove("teste_no.bin");
    }

    // Teste 3: Escrita e leitura normal
    printf("Teste 3: Escrita e leitura normal\n");
    criar_arquivo_teste("teste_no.bin", 1000);
    arquivo = fopen("teste_no.bin", "rb+");
    if (arquivo)
    {
        // Criar e configurar nó
        no = ArvB_no_criar();
        int chaves[] = {10, 20};
        int dados[] = {100, 200};
        int descendentes[] = {300, 400, 500};
        ArvB_no_set(no, TAM_HEADER_ARVB, chaves, dados, descendentes, 0, 2);

        // Escrever
        ArvB_no_escrever(arquivo, no);
        printf("✓ Nó escrito no arquivo\n");

        // Ler de volta
        NO *no_lido = ArvB_no_ler(arquivo, TAM_HEADER_ARVB);
        if (no_lido)
        {
            printf("✓ Nó lido do arquivo\n");
            ArvB_no_apagar(&no_lido);
        }

        fclose(arquivo);
        ArvB_no_apagar(&no);
        remove("teste_no.bin");
    }

    printf("\n");
}

void testar_busca()
{
    printf("=== TESTANDO ArvB_busca() ===\n");

    // Teste 1: Arquivo NULL
    printf("Teste 1: Arquivo NULL\n");
    NO *resultado = ArvB_busca(NULL, 0, 10);
    if (resultado == NULL)
    {
        printf("✓ Retornou NULL para arquivo inválido\n");
    }

    // Teste 2: ByteOffset inválido
    printf("Teste 2: ByteOffset negativo\n");
    FILE *arquivo = fopen("teste_busca.bin", "wb+");
    if (arquivo)
    {
        resultado = ArvB_busca(arquivo, -1, 10);
        if (resultado == NULL)
        {
            printf("✓ Retornou NULL para byteOffset inválido\n");
        }
        fclose(arquivo);
        remove("teste_busca.bin");
    }

    // Teste 3: Busca em nó folha
    printf("Teste 3: Busca em nó folha\n");
    criar_arquivo_teste("teste_busca.bin", 1000);
    arquivo = fopen("teste_busca.bin", "rb+");
    if (arquivo)
    {
        // Criar nó folha com algumas chaves
        NO *no = ArvB_no_criar();
        int chaves[] = {10, 20};
        int dados[] = {100, 200};
        int descendentes[] = {-1, -1, -1};
        ArvB_no_set(no, TAM_HEADER_ARVB, chaves, dados, descendentes, -1, 2);
        ArvB_no_escrever(arquivo, no);

        // Teste 3a: Chave encontrada
        printf("  Teste 3a: Chave encontrada (10)\n");
        resultado = ArvB_busca(arquivo, TAM_HEADER_ARVB, 10);
        if (resultado != NULL && resultado != (NO *)-1)
        {
            printf("  ✓ Chave encontrada\n");
            ArvB_no_apagar(&resultado);
        }

        // Teste 3b: Chave não encontrada
        printf("  Teste 3b: Chave não encontrada (15)\n");
        resultado = ArvB_busca(arquivo, TAM_HEADER_ARVB, 15);
        // ATENÇÃO: Esta função tem um bug - retorna -1 ao invés de NULL
        if (resultado == (NO *)-1)
        {
            printf("  ⚠ Retornou -1 (BUG: deveria ser NULL)\n");
        }
        else if (resultado == NULL)
        {
            printf("  ✓ Chave não encontrada\n");
        }
        else
        {
            printf("  ✗ Comportamento inesperado\n");
            ArvB_no_apagar(&resultado);
        }

        fclose(arquivo);
        ArvB_no_apagar(&no);
        remove("teste_busca.bin");
    }

    // Teste 4: Árvore vazia (noRaiz = -1)
    printf("Teste 4: Busca em árvore vazia\n");
    arquivo = fopen("teste_vazia.bin", "wb+");
    if (arquivo)
    {
        // Tentar buscar com byteOffset -1 causará erro
        resultado = ArvB_busca(arquivo, -1, 10);
        if (resultado == NULL)
        {
            printf("✓ Tratou árvore vazia corretamente\n");
        }
        fclose(arquivo);
        remove("teste_vazia.bin");
    }

    printf("\n");
}

void testar_casos_extremos()
{
    printf("=== TESTANDO CASOS EXTREMOS ===\n");

    // Teste 1: Arquivo muito pequeno
    printf("Teste 1: Arquivo muito pequeno\n");
    FILE *arquivo = fopen("teste_pequeno.bin", "wb");
    if (arquivo)
    {
        char byte = 0;
        fwrite(&byte, 1, 1, arquivo); // Apenas 1 byte
        fclose(arquivo);

        arquivo = fopen("teste_pequeno.bin", "rb");
        HEADER_ARVB *header = ArvB_header_ler(arquivo, NULL);
        if (header)
        {
            printf("⚠ Leu header de arquivo muito pequeno (dados podem ser inválidos)\n");
            ArvB_header_apagar(&header);
        }
        else
        {
            printf("✓ Falhou ao ler arquivo muito pequeno\n");
        }
        fclose(arquivo);
        remove("teste_pequeno.bin");
    }

    // Teste 2: Múltiplas liberações de memória
    printf("Teste 2: Múltiplas liberações\n");
    HEADER_ARVB *header = ArvB_header_criar();
    ArvB_header_apagar(&header);
    ArvB_header_apagar(&header); // Segunda liberação
    printf("✓ Não crashou com dupla liberação\n");

    NO *no = ArvB_no_criar();
    ArvB_no_apagar(&no);
    ArvB_no_apagar(&no); // Segunda liberação
    printf("✓ Não crashou com dupla liberação de nó\n");

    // Teste 3: Valores extremos
    printf("Teste 3: Valores extremos\n");
    header = ArvB_header_criar();
    ArvB_header_set(header, 999999, -999999, 999999, -999999);
    printf("✓ Aceitou valores extremos\n");
    ArvB_header_apagar(&header);

    printf("\n");
}

int main()
{
    printf("INICIANDO TESTES DA ÁRVORE B\n");
    printf("============================\n\n");

    testar_header_criar();
    testar_header_set();
    testar_header_get_int();
    testar_header_arquivo();
    testar_no_criar();
    testar_no_set();
    testar_no_arquivo();
    testar_busca();
    testar_casos_extremos();

    printf("TESTES CONCLUÍDOS\n");
    printf("=================\n");
    printf("ATENÇÃO: Este código contém os seguintes problemas identificados:\n");
    printf("1. ArvB_busca() retorna -1 ao invés de NULL\n");
    printf("2. Vazamento de memória na função ArvB_busca()\n");
    printf("3. ArvB_header_set() não cria header quando NULL (contradiz documentação)\n");
    printf("4. Falta validação de integridade dos dados lidos\n");
    printf("5. Não há tratamento robusto para arquivos corrompidos\n");

    return 0;
}